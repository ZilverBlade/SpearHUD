workspace "GammaHUD"
   architecture "x86_64"
   preferredtoolarchitecture "x86_64"
   configurations { "Debug", "Release" }

local outputSubfolder = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

local VULKAN_SDK = os.getenv("VULKAN_SDK") -- 

project "GHUDCore"
   location "./GHUDCore"
   debugdir "./"
   local bin = "./GHUDCore/bin/"..outputSubfolder

   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   targetdir (bin)

   flags { "MultiProcessorCompile" }
   disablewarnings { "26812;4244;4996;4005" }
   
   files {
      "./GHUDCore/include/ghudcpp/**.h",
      "./GHUDCore/src/**.cpp",
   }

   includedirs {
	  "./GHUDCore/include"
   }
   libdirs {
   }
   links {
   }

   filter { "system:windows" }
      links {
	   "shlwapi"
      }
	
   filter "configurations:Debug"
      defines { "_DEBUG", "_LIB" }
      runtime "Debug"
      symbols "On"
	  links {
	  }

   filter "configurations:Release"
	  defines { "NDEBUG" }
      runtime "Release"
      optimize "Speed"
	  links {
	  }
	  

project "GHUDVulkan"
   location "./GHUDVulkan"
   debugdir "./"
   local bin = "./GHUDVulkan/bin/"..outputSubfolder

   kind "SharedLib"
   language "C++"
   cppdialect "C++17"
   targetdir (bin)

   flags { "MultiProcessorCompile" }
   disablewarnings { "26812;4244;4996;4005" }
   
   files {
      "./GHUDVulkan/include/ghudvk/**.h",
      "./GHUDVulkan/src/**.cpp"
   }

   includedirs {
      VULKAN_SDK.."/Include"
   }
   libdirs {
      VULKAN_SDK.."/Lib"
   }
   links {
      "vulkan-1"
   }
	
   filter "configurations:Debug"
      defines { "_DEBUG", "_DLLEXPORT" }
      runtime "Debug"
      symbols "On"
	  links {
	  }

   filter "configurations:Release"
	  defines { "NDEBUG", "_DLLEXPORT" }
      runtime "Release"
      optimize "Speed"
	  links {
	  }