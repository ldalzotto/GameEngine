#include "IMGUITest.h"

#include "glm/glm.hpp"

#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"
#include "RenderInterface.h"
#include "VulkanObjects/Hardware/Device/Device.h"
#include "VulkanObjects/SwapChain/SwapChain.h"
#include "Texture/Texture.h"
#include "VulkanObjects/Hardware/Window/Window.h"
#include "VulkanObjects/Descriptor/DescriptorPool.h"

namespace _GameEngine::_Render
{
	void createFinalDrawObjects(RenderInterface* p_renderInterface, IMGUITest* p_imguiTest);

	void IMGUITest_init(IMGUITest* p_imguiTest, RenderInterface* p_renderInterface)
	{
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

		DescriptorPoolAllocInfo l_descritproPoolAllocInfo{};
		l_descritproPoolAllocInfo.MaxSet = 1000 * pool_sizes.size();
		l_descritproPoolAllocInfo.SourceDescriptorPoolSize = &pool_sizes;

		DescriptorPool_alloc(&p_imguiTest->DescriptorPool, p_renderInterface->Device, &l_descritproPoolAllocInfo);

		createFinalDrawObjects(p_renderInterface, p_imguiTest);

	};

	void IMGUITest_free(IMGUITest* p_imguiTest, RenderInterface* p_renderInterface)
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		RenderPass_free(&p_imguiTest->Renderpass);
		for (size_t i = 0; i < p_imguiTest->FrameBuffers.size(); i++)
		{
			FrameBuffer_free(&p_imguiTest->FrameBuffers.at(i));
		}
		p_imguiTest->FrameBuffers.clear();

		DescriptorPool_free(&p_imguiTest->DescriptorPool, p_renderInterface->Device);
		p_imguiTest->FontInitialized = false;


	};

	void IMGUITest_onSwapChainRebuilded(IMGUITest* p_imguiTest, RenderInterface* p_renderInterface)
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		RenderPass_free(&p_imguiTest->Renderpass);
		for (size_t i = 0; i < p_imguiTest->FrameBuffers.size(); i++)
		{
			FrameBuffer_free(&p_imguiTest->FrameBuffers.at(i));
		}
		p_imguiTest->FrameBuffers.clear();


		createFinalDrawObjects(p_renderInterface, p_imguiTest);

		p_imguiTest->FontInitialized = false;
	};

	void drawImguiElements();

	void IMGUITest_drawFrame(IMGUITest* p_imguiTest, VkCommandBuffer p_commandBuffer, size_t l_imageIndex, RenderInterface* p_renderInterface)
	{
		if (!p_imguiTest->FontInitialized)
		{
			ImGui_ImplVulkan_CreateFontsTexture(p_commandBuffer);
			p_imguiTest->FontInitialized = true;
		}

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		drawImguiElements();
		//ImGui::ShowDemoWindow();

		VkRenderPassBeginInfo l_renderPassBeginInfo{};
		l_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		l_renderPassBeginInfo.renderPass = p_imguiTest->Renderpass.renderPass;
		l_renderPassBeginInfo.framebuffer = p_imguiTest->FrameBuffers.at(l_imageIndex).FrameBuffer;
		l_renderPassBeginInfo.renderArea.offset = { 0,0 };
		l_renderPassBeginInfo.renderArea.extent = p_renderInterface->SwapChain->SwapChainInfo.SwapExtend;
		l_renderPassBeginInfo.clearValueCount = 0;
		l_renderPassBeginInfo.pClearValues = nullptr;

		vkCmdBeginRenderPass(p_commandBuffer, &l_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), p_commandBuffer);

		vkCmdEndRenderPass(p_commandBuffer);
	};

	void createFinalDrawObjects(RenderInterface* p_renderInterface, IMGUITest* p_imguiTest)
	{
		{
			RenderPassDependencies l_renderPassDependencies{};
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

			RenderPassBuildInfo l_renderPassBuildInfo{};
			l_renderPassBuildInfo.RenderPassDependencies = &l_renderPassDependencies;
			l_renderPassBuildInfo.ColorAttachmentDescription = &l_colorAttachmentDescription;
			RenderPass_build(&p_imguiTest->Renderpass, &l_renderPassBuildInfo);

		}

		{
			size_t l_imageCount = p_renderInterface->SwapChain->SwapChainImages.size();
			p_imguiTest->FrameBuffers.resize(l_imageCount);
			for (size_t i = 0; i < l_imageCount; i++)
			{
				FrameBufferDependencies l_frameBufferDependencies{};
				l_frameBufferDependencies.ColorImageView = &p_renderInterface->SwapChain->SwapChainImages.at(i).ImageView;
				l_frameBufferDependencies.Device = p_renderInterface->Device;
				l_frameBufferDependencies.SwapChainInfo = &p_renderInterface->SwapChain->SwapChainInfo;
				l_frameBufferDependencies.RenderPass = &p_imguiTest->Renderpass;
				FrameBuffer_init(&p_imguiTest->FrameBuffers.at(i), &l_frameBufferDependencies);
			}
		}

		{
			ImGui_ImplGlfw_InitForVulkan(p_renderInterface->Window->Window, true);

			ImGui_ImplVulkan_InitInfo l_imguiImplInfo{};
			l_imguiImplInfo.DescriptorPool = p_imguiTest->DescriptorPool.DescriptorPool;
			l_imguiImplInfo.Device = p_renderInterface->Device->LogicalDevice.LogicalDevice;
			l_imguiImplInfo.PhysicalDevice = p_renderInterface->Device->PhysicalDevice.PhysicalDevice;
			l_imguiImplInfo.ImageCount = p_renderInterface->SwapChain->SwapChainImages.size();
			l_imguiImplInfo.MinImageCount = p_renderInterface->SwapChain->SwapChainImages.size();
			l_imguiImplInfo.Instance = *p_renderInterface->Instance;
			l_imguiImplInfo.Queue = p_renderInterface->Device->LogicalDevice.Queues.GraphicsQueue;
			l_imguiImplInfo.QueueFamily = p_renderInterface->Device->PhysicalDevice.QueueFamilies.Graphics.QueueIndex;
			ImGui_ImplVulkan_Init(&l_imguiImplInfo, p_imguiTest->Renderpass.renderPass);
		}

		ImGuiIO& l_io = ImGui::GetIO();
		l_io.DisplaySize.x = p_renderInterface->SwapChain->SwapChainInfo.SwapExtend.width;
		l_io.DisplaySize.y = p_renderInterface->SwapChain->SwapChainInfo.SwapExtend.height;
	};

	bool my_tool_active;
	glm::vec4 my_color;

	void drawImguiElements()
	{
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Edit a color (stored as ~4 floats)
		ImGui::ColorEdit4("Color", &my_color.r);

		// Plot some values
		const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
		ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

		// Display contents in a scrolling region
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
		ImGui::BeginChild("Scrolling");
		for (int n = 0; n < 50; n++)
		{
			ImGui::Text("%04d: Some text", n);
		}
		ImGui::EndChild();
		ImGui::End();
	}
}