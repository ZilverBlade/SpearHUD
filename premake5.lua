workspace "SpearHUD"
   architecture "x86_64"
   preferredtoolarchitecture "x86_64"
   configurations { "Debug", "Release" }

local VULKAN_SDK = os.getenv("VULKAN_SDK") -- 

project "SHUDCore"
   location "./SHUDCore"
   debugdir "./"
   local bin = "./SHUDCore/lib"

   kind "StaticLib"
   language "C++"
   cppdialect "C++14"
   targetdir (bin)

   flags { "MultiProcessorCompile" }
   disablewarnings { "26812;4244;4996;4005" }
   
   files {
      "./SHUDCore/include/shudcpp/**.h",
      "./SHUDCore/include/shudcpp/draw/**.h",
      "./SHUDCore/include/shudcpp/resources/**.h",
      "./SHUDCore/src/**.cpp",
   }

   includedirs {
	  "./SHUDCore/include"
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
      targetname "SHUDCore_d"
      defines { "_DEBUG", "_LIB" }
      runtime "Debug"
      symbols "On"
	  links {
	  }

   filter "configurations:Release"
      targetname "SHUDCore"
	  defines { "NDEBUG" }
      runtime "Release"
      optimize "Speed"
	  links {
	  }
	  

project "SHUDVulkan"
   location "./SHUDVulkan"
   debugdir "./"
   local bin = "./SHUDVulkan/lib"

   kind "StaticLib"
   language "C++"
   cppdialect "C++14"
   targetdir (bin)

   flags { "MultiProcessorCompile" }
   disablewarnings { "26812;4244;4996;4005" }
   
   files {
      "./SHUDVulkan/include/shudvk/**.h",
      "./SHUDVulkan/include/shudvk/src_impl/**.hh"
   }

   includedirs {
      VULKAN_SDK.."/Include",
	  "./SHUDCore/include",
	  "./SHUDVulkan/include"
   }
   libdirs {
   }
   links {
   }
	
   filter "configurations:Debug"
      targetname "SHUDVulkan_d"
      defines { "_DEBUG" }
      runtime "Debug"
      symbols "On"
	  links {
	  }

   filter "configurations:Release"
      targetname "SHUDVulkan"
	  defines { "NDEBUG" }
      runtime "Release"
      optimize "Speed"
	  links {
	  }
	  
project "SHUDOpenGL4"
   location "./SHUDOpenGL4"
   debugdir "./"
   local bin = "./SHUDOpenGL4/lib"

   kind "StaticLib"
   language "C++"
   cppdialect "C++14"
   targetdir (bin)

   flags { "MultiProcessorCompile" }
   disablewarnings { "26812;4244;4996;4005" }
   
   files {
      "./SHUDOpenGL4/include/shudgl4/**.h",
      "./SHUDOpenGL4/src/**.cpp"
   }

   includedirs {
	  "./SHUDCore/include",
	  "./SHUDOpenGL4/include"
   }
   libdirs {
   }
   links {
   }
	
   filter "configurations:Debug"
      targetname "SHUDOpenGL4_d"
      defines { "_DEBUG" }
      runtime "Debug"
      symbols "On"
	  links {
	  }

   filter "configurations:Release"
      targetname "SHUDOpenGL4"
	  defines { "NDEBUG" }
      runtime "Release"
      optimize "Speed"
	  links {
	  }