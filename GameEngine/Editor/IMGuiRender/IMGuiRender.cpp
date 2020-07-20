#include "IMGuiRender.h"

#include "Render/Render.h"

#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

#include "GameEngineApplicationInterface.h"
#include "RenderInterface.h"

#include "RenderHook.h"
#include "VulkanObjects/Hardware/Device/Device.h"
#include "VulkanObjects/SwapChain/SwapChain.h"
#include "Texture/Texture.h"
#include "VulkanObjects/Hardware/Window/Window.h"
#include "VulkanObjects/Descriptor/DescriptorPool.h"
#include "VulkanObjects/CommandBuffer/CommandBufferSingleExecution.h"

namespace _GameEngineEditor
{
	void newFrame(IMGuiRender* p_IMGuiRender, GameEngineApplicationInterface* p_gameEngineApplication);
	void drawFrame(IMGuiRender* p_IMGuiRender, _Render::BeforeEndRecordingMainCommandBuffer_Input* p_beforeEndRecordingMainCommandBuffer_Input);
	void createFinalDrawObjects(_Render::RenderInterface* p_renderInterface, IMGuiRender* p_IMGuiRender);
	void onSwapChainRebuilded(IMGuiRender* p_IMGuiRender, _Render::RenderInterface* p_renderInterface);

	void IMGuiRender_init(IMGuiRender* p_IMGuiRender, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		_Render::RenderInterface* p_renderInterface = p_gameEngineApplicationInterface->RenderInterface;

		p_IMGuiRender->NewFrame = { newFrame, p_IMGuiRender };
		_Core::ObserverT_register(p_gameEngineApplicationInterface->NewFrame, (_Core::CallbackT<void, GameEngineApplicationInterface>*) &p_IMGuiRender->NewFrame);

		p_IMGuiRender->DrawFrame = { drawFrame, p_IMGuiRender };
		_Core::ObserverT_register(&p_renderInterface->RenderHookCallbacksInterface.RenderHookCallbacks->BeforeEndRecordingMainCommandBuffer, 
				(_Core::CallbackT<void, _Render::BeforeEndRecordingMainCommandBuffer_Input>*) &p_IMGuiRender->DrawFrame);

		p_IMGuiRender->SwapChainRebuild = { onSwapChainRebuilded, p_IMGuiRender };
		_Core::ObserverT_register(&p_renderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _Render::RenderInterface>*) &p_IMGuiRender->SwapChainRebuild);

		ImGuiContext* l_imGuicontext = ImGui::CreateContext();
		ImGui::SetCurrentContext(l_imGuicontext);
		ImGui::StyleColorsClassic();

		std::vector<VkDescriptorPoolSize > pool_sizes =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		_Render::DescriptorPoolAllocInfo l_descritproPoolAllocInfo{};
		l_descritproPoolAllocInfo.MaxSet = 1000 * pool_sizes.size();
		l_descritproPoolAllocInfo.SourceDescriptorPoolSize = &pool_sizes;

		DescriptorPool_alloc(&p_IMGuiRender->DescriptorPool, p_renderInterface->Device, &l_descritproPoolAllocInfo);

		createFinalDrawObjects(p_renderInterface, p_IMGuiRender);

	};

	void IMGuiRender_free(IMGuiRender* p_IMGuiRender, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		_Core::ObserverT_unRegister(p_gameEngineApplicationInterface->NewFrame, (_Core::CallbackT<void, GameEngineApplicationInterface>*) &p_IMGuiRender->NewFrame);

		_Render::RenderInterface* l_renderInterface = p_gameEngineApplicationInterface->RenderInterface;

		_Core::ObserverT_unRegister(&l_renderInterface->RenderHookCallbacksInterface.RenderHookCallbacks->BeforeEndRecordingMainCommandBuffer, 
			(_Core::CallbackT<void, _Render::BeforeEndRecordingMainCommandBuffer_Input>*)  &p_IMGuiRender->DrawFrame);
		_Core::ObserverT_unRegister(&l_renderInterface->SwapChain->OnSwapChainBuilded, (_Core::CallbackT<void, _Render::RenderInterface>*) &p_IMGuiRender->SwapChainRebuild);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		RenderPass_free(&p_IMGuiRender->Renderpass);
		for (size_t i = 0; i < p_IMGuiRender->FrameBuffers.size(); i++)
		{
			FrameBuffer_free(&p_IMGuiRender->FrameBuffers.at(i));
		}
		p_IMGuiRender->FrameBuffers.clear();

		DescriptorPool_free(&p_IMGuiRender->DescriptorPool, l_renderInterface->Device);
		p_IMGuiRender->FontInitialized = false;
	};

	void onSwapChainRebuilded(IMGuiRender* p_IMGuiRender, _Render::RenderInterface* p_renderInterface)
	{
		ImGui::EndFrame();

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		RenderPass_free(&p_IMGuiRender->Renderpass);
		for (size_t i = 0; i < p_IMGuiRender->FrameBuffers.size(); i++)
		{
			FrameBuffer_free(&p_IMGuiRender->FrameBuffers.at(i));
		}
		p_IMGuiRender->FrameBuffers.clear();

		createFinalDrawObjects(p_renderInterface, p_IMGuiRender);

		p_IMGuiRender->FontInitialized = false;
	};

	void newFrame(IMGuiRender* p_IMGuiRender, GameEngineApplicationInterface* p_gameEngineApplication)
	{
		_Render::RenderInterface* l_renderInterface = p_gameEngineApplication->RenderInterface;

		if (!p_IMGuiRender->FontInitialized)
		{
			_Render::CommandBufferSingleExecution l_commandBufferSingleExecution{};
			{
				_Render::CommandBufferSingleExecutionAllocInfo l_allocInfo{};
				l_allocInfo.Queue = l_renderInterface->Device->LogicalDevice.Queues.GraphicsQueue;
				CommandBufferSingleExecution_alloc(&l_commandBufferSingleExecution, l_renderInterface->CommandPool, l_renderInterface->Device, &l_allocInfo);
			}

			CommandBufferSingleExecution_startRecording(&l_commandBufferSingleExecution);
			ImGui_ImplVulkan_CreateFontsTexture(l_commandBufferSingleExecution.CommandBuffer.CommandBuffer);
			CommandBufferSingleExecution_execute(&l_commandBufferSingleExecution, l_renderInterface->Device);
			p_IMGuiRender->FontInitialized = true;
		}

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

	};

	void drawFrame(IMGuiRender* p_IMGuiRender, _Render::BeforeEndRecordingMainCommandBuffer_Input* p_beforeEndRecordingMainCommandBuffer_Input)
	{
		if (p_IMGuiRender->FontInitialized)
		{
			VkRenderPassBeginInfo l_renderPassBeginInfo{};
			l_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			l_renderPassBeginInfo.renderPass = p_IMGuiRender->Renderpass.renderPass;
			l_renderPassBeginInfo.framebuffer = p_IMGuiRender->FrameBuffers.at(p_beforeEndRecordingMainCommandBuffer_Input->ImageIndex).FrameBuffer;
			l_renderPassBeginInfo.renderArea.offset = { 0,0 };
			l_renderPassBeginInfo.renderArea.extent = p_beforeEndRecordingMainCommandBuffer_Input->RenderInterface->SwapChain->SwapChainInfo.SwapExtend;
			l_renderPassBeginInfo.clearValueCount = 0;
			l_renderPassBeginInfo.pClearValues = nullptr;

			vkCmdBeginRenderPass(p_beforeEndRecordingMainCommandBuffer_Input->CommandBuffer, &l_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			ImGui::Render();
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), p_beforeEndRecordingMainCommandBuffer_Input->CommandBuffer);

			vkCmdEndRenderPass(p_beforeEndRecordingMainCommandBuffer_Input->CommandBuffer);
		}


	};

	void createFinalDrawObjects(_Render::RenderInterface* p_renderInterface, IMGuiRender* p_IMGuiRender)
	{
		{
			_Render::RenderPassDependencies l_renderPassDependencies{};
			l_renderPassDependencies.SwapChain = p_renderInterface->SwapChain;

			VkAttachmentDescription l_colorAttachmentDescription{};
			l_colorAttachmentDescription.format = p_renderInterface->SwapChain->SwapChainInfo.SurfaceFormat.format;
			l_colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
			l_colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			l_colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			l_colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			l_colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			l_colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			l_colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			_Render::RenderPassBuildInfo l_renderPassBuildInfo{};
			l_renderPassBuildInfo.RenderPassDependencies = &l_renderPassDependencies;
			l_renderPassBuildInfo.ColorAttachmentDescription = &l_colorAttachmentDescription;
			RenderPass_build(&p_IMGuiRender->Renderpass, &l_renderPassBuildInfo);

		}

		{
			size_t l_imageCount = p_renderInterface->SwapChain->SwapChainImages.size();
			p_IMGuiRender->FrameBuffers.resize(l_imageCount);
			for (size_t i = 0; i < l_imageCount; i++)
			{
				_Render::FrameBufferDependencies l_frameBufferDependencies{};
				l_frameBufferDependencies.ColorImageView = &p_renderInterface->SwapChain->SwapChainImages.at(i).ImageView;
				l_frameBufferDependencies.Device = p_renderInterface->Device;
				l_frameBufferDependencies.SwapChainInfo = &p_renderInterface->SwapChain->SwapChainInfo;
				l_frameBufferDependencies.RenderPass = &p_IMGuiRender->Renderpass;
				FrameBuffer_init(&p_IMGuiRender->FrameBuffers.at(i), &l_frameBufferDependencies);
			}
		}

		{
			ImGui_ImplGlfw_InitForVulkan(p_renderInterface->Window->Window, true);

			ImGui_ImplVulkan_InitInfo l_imguiImplInfo{};
			l_imguiImplInfo.DescriptorPool = p_IMGuiRender->DescriptorPool.DescriptorPool;
			l_imguiImplInfo.Device = p_renderInterface->Device->LogicalDevice.LogicalDevice;
			l_imguiImplInfo.PhysicalDevice = p_renderInterface->Device->PhysicalDevice.PhysicalDevice;
			l_imguiImplInfo.ImageCount = p_renderInterface->SwapChain->SwapChainImages.size();
			l_imguiImplInfo.MinImageCount = p_renderInterface->SwapChain->SwapChainImages.size();
			l_imguiImplInfo.Instance = *p_renderInterface->Instance;
			l_imguiImplInfo.Queue = p_renderInterface->Device->LogicalDevice.Queues.GraphicsQueue;
			l_imguiImplInfo.QueueFamily = p_renderInterface->Device->PhysicalDevice.QueueFamilies.Graphics.QueueIndex;
			ImGui_ImplVulkan_Init(&l_imguiImplInfo, p_IMGuiRender->Renderpass.renderPass);
		}

		ImGuiIO& l_io = ImGui::GetIO();
		l_io.DisplaySize.x = p_renderInterface->SwapChain->SwapChainInfo.SwapExtend.width;
		l_io.DisplaySize.y = p_renderInterface->SwapChain->SwapChainInfo.SwapExtend.height;
	};
}