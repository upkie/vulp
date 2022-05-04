# -*- python -*-
#
# Copyright 2022 Stéphane Caron

package(default_visibility = ["//visibility:public"])

# Disable common Bullet warnings
bullet_copts = [
    "-Wno-all",
    "-Wno-error=unused-but-set-variable",
    "-Wno-error=unused-variable",
    "-Wno-format-overflow",
]

cc_library(
    name = "src",
    srcs = glob([
        "src/Bullet3Common/**/*.cpp",
        "src/BulletCollision/**/*.cpp",
        "src/BulletDynamics/**/*.cpp",
        "src/BulletInverseDynamics/**/*.cpp",
        "src/BulletSoftBody/**/*.cpp",
        "src/LinearMath/**/*.cpp",
    ]),
    hdrs = glob([
        "src/Bullet3Collision/**/*.h",
        "src/Bullet3Common/**/*.h",
        "src/BulletCollision/**/*.h",
        "src/BulletDynamics/**/*.h",
        "src/BulletInverseDynamics/**/*.hpp",
        "src/BulletSoftBody/**/*.h",
        "src/LinearMath/**/*.h",
        "src/btBulletCollisionCommon.h",
        "src/btBulletDynamicsCommon.h",
    ]),
    defines = ["BT_USE_DOUBLE_PRECISION"],
    includes = ["src"],
    copts = bullet_copts,
)

cc_library(
    name = "glad",
    srcs = glob([
        "examples/ThirdPartyLibs/glad/**/*.cpp",
    ]),
    hdrs = glob([
        "examples/ThirdPartyLibs/glad/**/*.h",
    ]),
    strip_include_prefix = "examples/ThirdPartyLibs/glad",
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "gwen",
    srcs = glob([
        "examples/ThirdPartyLibs/Gwen/**/*.cpp",
    ]),
    hdrs = glob([
        "examples/ThirdPartyLibs/Gwen/**/*.h",
    ]),
    deps = [
        ":glad",
    ],
    includes = ["examples/ThirdPartyLibs"],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "extras",
    srcs = glob([
        "Extras/InverseDynamics/**/*.cpp",
        "Extras/Serialize/BulletFileLoader/**/*.cpp",
    ]),
    hdrs = glob([
        "Extras/InverseDynamics/**/*.hpp",
        "Extras/Serialize/BulletFileLoader/**/*.h",
    ]),
    deps = [
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "common_interfaces",
    hdrs = [
        "examples/CommonInterfaces/Common2dCanvasInterface.h",
        "examples/CommonInterfaces/CommonCallbacks.h",
        "examples/CommonInterfaces/CommonCameraInterface.h",
        "examples/CommonInterfaces/CommonExampleInterface.h",
        "examples/CommonInterfaces/CommonFileIOInterface.h",
        "examples/CommonInterfaces/CommonGUIHelperInterface.h",
        "examples/CommonInterfaces/CommonGraphicsAppInterface.h",
        "examples/CommonInterfaces/CommonMultiBodyBase.h",
        "examples/CommonInterfaces/CommonParameterInterface.h",
        "examples/CommonInterfaces/CommonRenderInterface.h",
        "examples/CommonInterfaces/CommonRigidBodyBase.h",
        "examples/CommonInterfaces/CommonWindowInterface.h",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "stb_image",
    srcs = glob(["examples/ThirdPartyLibs/stb_image/*.cpp"]),
    hdrs = glob(["examples/ThirdPartyLibs/stb_image/*.h"]),
    includes = ["examples/ThirdPartyLibs"],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "opengl_window",
    srcs = [
        "examples/OpenGLWindow/GLInstancingRenderer.cpp",
        "examples/OpenGLWindow/GLPrimitiveRenderer.cpp",
        "examples/OpenGLWindow/SimpleCamera.cpp",
        "examples/OpenGLWindow/SimpleOpenGL2App.cpp",
        "examples/OpenGLWindow/SimpleOpenGL3App.cpp",
        "examples/OpenGLWindow/TwFonts.cpp",
        "examples/OpenGLWindow/X11OpenGLWindow.cpp",
    ],
    hdrs = glob([
        "examples/OpenGLWindow/**/*.h",
        # "examples/OpenGLWindow/GLInstanceRendererInternalData.h",
        # "examples/OpenGLWindow/GLInstancingRenderer.h",
        # "examples/OpenGLWindow/GLPrimInternalData.h",
        # "examples/OpenGLWindow/GLPrimitiveRenderer.h",
        # "examples/OpenGLWindow/LoadShader.h",
        # "examples/OpenGLWindow/OpenGLInclude.h",
        # "examples/OpenGLWindow/ShapeData.h",
        # "examples/OpenGLWindow/SimpleCamera.h",
        # "examples/OpenGLWindow/SimpleOpenGL2App.h",
        # "examples/OpenGLWindow/SimpleOpenGL3App.h",
        # "examples/OpenGLWindow/Shaders/*.h",
        # "examples/OpenGLWindow/TwFonts.h",
        # "examples/OpenGLWindow/X11OpenGLWindow.h",
    ]),
    defines = ["BT_USE_DOUBLE_PRECISION"],
    deps = [
        ":common_interfaces",
        ":glad",
        ":src",
        ":stb_image",
    ],
    copts = bullet_copts,
)

cc_library(
    name = "gwen_gui_support",
    srcs = [
        "examples/ExampleBrowser/GwenGUISupport/GraphingTexture.cpp",
    ],
    hdrs = [
        "examples/ExampleBrowser/GwenGUISupport/GraphingTexture.h",
    ],
    deps = [
        ":opengl_window",
        ":src",
    ],
    strip_include_prefix = "examples/ExampleBrowser",
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "tinyxml2",
    srcs = glob(["examples/ThirdPartyLibs/tinyxml2/*.cpp"]),
    hdrs = glob(["examples/ThirdPartyLibs/tinyxml2/*.h"]),
    includes = [
        "examples/ThirdPartyLibs",
    ],
    deps = [
        ":common_interfaces",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "wavefront",
    srcs = glob([
        "examples/ThirdPartyLibs/Wavefront/tiny_obj_loader.cpp"]),
    hdrs = glob([
        "examples/ThirdPartyLibs/Wavefront/tiny_obj_loader.h",
    ]),
    includes = [
        "examples/ThirdPartyLibs",
    ],
    deps = [
        ":common_interfaces",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "utils",
    srcs = [
        "examples/Utils/ChromeTraceUtil.cpp",
        "examples/Utils/b3Clock.cpp",
        "examples/Utils/b3ResourcePath.cpp",
    ],
    hdrs = [
        "examples/Utils/ChromeTraceUtil.h",
        "examples/Utils/b3BulletDefaultFileIO.h",
        "examples/Utils/b3Clock.h",
        "examples/Utils/b3ResourcePath.h",
    ],
    deps = [
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "importers",
    srcs = [
        "examples/Importers/ImportBsp/BspConverter.cpp",
        "examples/Importers/ImportBsp/BspLoader.cpp",
        "examples/Importers/ImportBsp/ImportBspExample.cpp",
        "examples/Importers/ImportBullet/SerializeSetup.cpp",
        "examples/Importers/ImportColladaDemo/ImportColladaSetup.cpp",
        "examples/Importers/ImportColladaDemo/LoadMeshFromCollada.cpp",
        "examples/Importers/ImportMJCFDemo/BulletMJCFImporter.cpp",
        "examples/Importers/ImportMJCFDemo/ImportMJCFSetup.cpp",
        "examples/Importers/ImportMeshUtility/b3ImportMeshUtility.cpp",
        "examples/Importers/ImportObjDemo/ImportObjExample.cpp",
        "examples/Importers/ImportObjDemo/LoadMeshFromObj.cpp",
        "examples/Importers/ImportObjDemo/Wavefront2GLInstanceGraphicsShape.cpp",
        "examples/Importers/ImportSDFDemo/ImportSDFSetup.cpp",
        "examples/Importers/ImportSTLDemo/ImportSTLSetup.cpp",
        "examples/Importers/ImportURDFDemo/BulletUrdfImporter.cpp",
        "examples/Importers/ImportURDFDemo/ImportURDFSetup.cpp",
        "examples/Importers/ImportURDFDemo/MyMultiBodyCreator.cpp",
        "examples/Importers/ImportURDFDemo/URDF2Bullet.cpp",
        "examples/Importers/ImportURDFDemo/UrdfParser.cpp",
        "examples/Importers/ImportURDFDemo/urdfStringSplit.cpp",
    ],
    hdrs = [
        "examples/Importers/ImportBsp/BspConverter.h",
        "examples/Importers/ImportBsp/BspLoader.h",
        "examples/Importers/ImportBsp/ImportBspExample.h",
        "examples/Importers/ImportBullet/SerializeSetup.h",
        "examples/Importers/ImportColladaDemo/ColladaGraphicsInstance.h",
        "examples/Importers/ImportColladaDemo/ImportColladaSetup.h",
        "examples/Importers/ImportColladaDemo/LoadMeshFromCollada.h",
        "examples/Importers/ImportColladaDemo/btMatrix4x4.h",
        "examples/Importers/ImportMJCFDemo/BulletMJCFImporter.h",
        "examples/Importers/ImportMJCFDemo/ImportMJCFSetup.h",
        "examples/Importers/ImportMeshUtility/b3ImportMeshUtility.h",
        "examples/Importers/ImportObjDemo/ImportObjExample.h",
        "examples/Importers/ImportObjDemo/LoadMeshFromObj.h",
        "examples/Importers/ImportObjDemo/Wavefront2GLInstanceGraphicsShape.h",
        "examples/Importers/ImportSDFDemo/ImportSDFSetup.h",
        "examples/Importers/ImportSTLDemo/ImportSTLSetup.h",
        "examples/Importers/ImportSTLDemo/LoadMeshFromSTL.h",
        "examples/Importers/ImportURDFDemo/BulletUrdfImporter.h",
        "examples/Importers/ImportURDFDemo/ImportURDFSetup.h",
        "examples/Importers/ImportURDFDemo/MultiBodyCreationInterface.h",
        "examples/Importers/ImportURDFDemo/MyMultiBodyCreator.h",
        "examples/Importers/ImportURDFDemo/SDFAudioTypes.h",
        "examples/Importers/ImportURDFDemo/URDF2Bullet.h",
        "examples/Importers/ImportURDFDemo/URDFImporterInterface.h",
        "examples/Importers/ImportURDFDemo/URDFJointTypes.h",
        "examples/Importers/ImportURDFDemo/UrdfFindMeshFile.h",
        "examples/Importers/ImportURDFDemo/UrdfParser.h",
        "examples/Importers/ImportURDFDemo/UrdfRenderingInterface.h",
        "examples/Importers/ImportURDFDemo/urdfLexicalCast.h",
        "examples/Importers/ImportURDFDemo/urdfStringSplit.h",
    ],
    deps = [
        ":common_interfaces",
        ":gwen_gui_support",
        ":src",
        ":tinyxml2",
        ":utils",
        ":wavefront",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "tiny_renderer",
    srcs = [
        "examples/TinyRenderer/TinyRenderer.cpp",
        "examples/TinyRenderer/geometry.cpp",
        "examples/TinyRenderer/our_gl.cpp",
        "examples/TinyRenderer/tgaimage.cpp",
    ],
    hdrs = [
        "examples/TinyRenderer/TinyRenderer.h",
        "examples/TinyRenderer/geometry.h",
        "examples/TinyRenderer/model.h",
        "examples/TinyRenderer/our_gl.h",
        "examples/TinyRenderer/tgaimage.h",
    ],
    deps = [
        ":common_interfaces",
        ":opengl_window",
        ":src",
        ":utils",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "rendering_examples",
    srcs = [
        "examples/RenderingExamples/CoordinateSystemDemo.cpp",
        "examples/RenderingExamples/DynamicTexturedCubeDemo.cpp",
        "examples/RenderingExamples/RaytracerSetup.cpp",
        "examples/RenderingExamples/RenderInstancingDemo.cpp",
        "examples/RenderingExamples/TimeSeriesCanvas.cpp",
        "examples/RenderingExamples/TinyVRGui.cpp",
    ],
    hdrs = [
        "examples/RenderingExamples/CoordinateSystemDemo.h",
        "examples/RenderingExamples/DynamicTexturedCubeDemo.h",
        "examples/RenderingExamples/RaytracerSetup.h",
        "examples/RenderingExamples/RenderInstancingDemo.h",
        "examples/RenderingExamples/TimeSeriesCanvas.h",
        "examples/RenderingExamples/TimeSeriesFontData.h",
        "examples/RenderingExamples/TinyVRGui.h",
    ],
    deps = [
        ":common_interfaces",
        ":gwen_gui_support",
        ":importers",
        ":src",
        ":tiny_renderer",
        ":tinyxml2",
        ":wavefront",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "multi_threading",
    srcs = [
        "examples/MultiThreading/b3PosixThreadSupport.cpp",
        "examples/MultiThreading/b3ThreadSupportInterface.cpp",
    ],
    hdrs = [
        "examples/MultiThreading/b3PosixThreadSupport.h",
        "examples/MultiThreading/b3ThreadSupportInterface.h",
    ],
    deps = [
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "forklift",
    srcs = [
        "examples/ForkLift/ForkLiftDemo.cpp",
    ],
    hdrs = [
        "examples/ForkLift/ForkLiftDemo.h",
    ],
    deps = [
        ":common_interfaces",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "basic_demo",
    srcs = [
        "examples/BasicDemo/BasicExample.cpp",
    ],
    hdrs = [
        "examples/BasicDemo/BasicExample.h",
    ],
    deps = [
        ":common_interfaces",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "gyroscopic_demo",
    srcs = [
    ],
    hdrs = [
        "examples/GyroscopicDemo/GyroscopicSetup.h",
    ],
    deps = [
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "planar_2d",
    srcs = [
        "examples/Planar2D/Planar2D.cpp",
    ],
    hdrs = [
        "examples/Planar2D/Planar2D.h",
    ],
    deps = [
        ":common_interfaces",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "multi_threaded_demo",
    srcs = [
        "examples/MultiThreadedDemo/MultiThreadedDemo.cpp",
    ],
    hdrs = [
        "examples/MultiThreadedDemo/CommonRigidBodyMTBase.h",
        "examples/MultiThreadedDemo/MultiThreadedDemo.h",
    ],
    deps = [
        ":common_interfaces",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "heightfield",
    srcs = [
        "examples/Heightfield/HeightfieldExample.cpp",
    ],
    hdrs = [
        "examples/Heightfield/HeightfieldExample.h",
    ],
    deps = [
        ":common_interfaces",
        ":importers",
        ":multi_threaded_demo",
        ":opengl_window",
        ":src",
        ":utils",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "benchmarks",
    srcs = [
        "examples/Benchmarks/BenchmarkDemo.cpp",
    ],
    hdrs = [
        "examples/Benchmarks/BenchmarkDemo.h",
        "examples/Benchmarks/HaltonData.h",
        "examples/Benchmarks/TaruData.h",
        "examples/Benchmarks/landscapeData.h",
    ],
    deps = [
        ":multi_threaded_demo",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "collision",
    srcs = glob([
        "examples/Collision/**/*.cpp",
    ]),
    hdrs = glob([
        "examples/Collision/**/*.h",
    ]),
    deps = [
        ":common_interfaces",
        ":rendering_examples",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "constraints",
    srcs = [
        "examples/Constraints/ConstraintDemo.cpp",
        "examples/Constraints/ConstraintPhysicsSetup.cpp",
        "examples/Constraints/Dof6Spring2Setup.cpp",
    ],
    hdrs = [
        "examples/Constraints/ConstraintDemo.h",
        "examples/Constraints/ConstraintPhysicsSetup.h",
        "examples/Constraints/Dof6Spring2Setup.h",
    ],
    deps = [
        ":common_interfaces",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "multi_body",
    srcs = [
        "examples/MultiBody/InvertedPendulumPDControl.cpp",
        "examples/MultiBody/KinematicMultiBodyExample.cpp",
        "examples/MultiBody/MultiBodyConstraintFeedback.cpp",
        "examples/MultiBody/MultiBodySoftContact.cpp",
        "examples/MultiBody/MultiDofDemo.cpp",
        "examples/MultiBody/Pendulum.cpp",
        "examples/MultiBody/TestJointTorqueSetup.cpp",
    ],
    hdrs = [
        "examples/MultiBody/InvertedPendulumPDControl.h",
        "examples/MultiBody/KinematicMultiBodyExample.h",
        "examples/MultiBody/MultiBodyConstraintFeedback.h",
        "examples/MultiBody/MultiBodySoftContact.h",
        "examples/MultiBody/MultiDofDemo.h",
        "examples/MultiBody/Pendulum.h",
        "examples/MultiBody/TestJointTorqueSetup.h",
    ],
    deps = [
        ":opengl_window",
        ":src",
        ":utils",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "rigid_body",
    srcs = [
        "examples/RigidBody/KinematicRigidBodyExample.cpp",
        "examples/RigidBody/RigidBodySoftContact.cpp",
    ],
    hdrs = [
        "examples/RigidBody/KinematicRigidBodyExample.h",
        "examples/RigidBody/RigidBodySoftContact.h",
    ],
    deps = [
        ":common_interfaces",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "voronoi_fracture",
    srcs = [
        "examples/VoronoiFracture/VoronoiFractureDemo.cpp",
        "examples/VoronoiFracture/btConvexConvexMprAlgorithm.cpp",
    ],
    hdrs = [
        "examples/VoronoiFracture/VoronoiFractureDemo.h",
        "examples/VoronoiFracture/btConvexConvexMprAlgorithm.h",
    ],
    deps = [
        ":common_interfaces",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "vehicles",
    srcs = [
        "examples/Vehicles/Hinge2Vehicle.cpp",
    ],
    hdrs = [
        "examples/Vehicles/Hinge2Vehicle.h",
    ],
    deps = [
        ":common_interfaces",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "shared_memory",
    srcs = [
        "examples/ExampleBrowser/CollisionShape2TriangleMesh.cpp",
        "examples/ExampleBrowser/GL_ShapeDrawer.cpp",
        "examples/ExampleBrowser/ExampleEntries.cpp",
        "examples/ExampleBrowser/InProcessExampleBrowser.cpp",  # yes...
        "examples/ExampleBrowser/OpenGLExampleBrowser.cpp",
        "examples/RenderingExamples/TinyRendererSetup.cpp",  # yes...
        "examples/SharedMemory/GraphicsClientExample.cpp",
        "examples/SharedMemory/GraphicsServerExample.cpp",
        "examples/SharedMemory/InProcessMemory.cpp",
        "examples/SharedMemory/PhysicsClientC_API.cpp",
        "examples/SharedMemory/PhysicsClientSharedMemory.cpp",
        "examples/SharedMemory/PhysicsClientSharedMemory2_C_API.cpp",
        "examples/SoftDemo/SoftDemo.cpp",
        "examples/SharedMemory/PhysicsClientSharedMemory_C_API.cpp",
        "examples/SharedMemory/PhysicsDirect.cpp",
        "examples/SharedMemory/PhysicsLoopBackC_API.cpp",
        "examples/SharedMemory/PhysicsServer.cpp",
        "examples/SharedMemory/PhysicsServerExample.cpp",
        "examples/SharedMemory/PhysicsServerExampleBullet2.cpp",  # needed
        "examples/SharedMemory/PhysicsServerSharedMemory.cpp",
        "examples/SharedMemory/PosixSharedMemory.cpp",
        "examples/SharedMemory/RemoteGUIHelper.cpp",
        "examples/SharedMemory/SharedMemoryCommandProcessor.cpp",
        "examples/SharedMemory/SharedMemoryInProcessPhysicsC_API.cpp",  # needed
        "examples/SharedMemory/Win32SharedMemory.cpp",
        "examples/SharedMemory/b3RobotSimulatorClientAPI_NoDirect.cpp",
        "examples/SharedMemory/b3RobotSimulatorClientAPI_NoGUI.cpp",
        # "examples/SharedMemory/IKTrajectoryHelper.cpp",
        # "examples/SharedMemory/PhysicsClientExample.cpp",
        # "examples/SharedMemory/PhysicsDirectC_API.cpp",
        # "examples/SharedMemory/PhysicsLoopBack.cpp",
        # "examples/SharedMemory/PhysicsServerCommandProcessor.cpp",
    ],
    hdrs = [
        "examples/ExampleBrowser/CollisionShape2TriangleMesh.h",
        "examples/ExampleBrowser/EmptyExample.h",
        "examples/ExampleBrowser/ExampleBrowserInterface.h",
        "examples/ExampleBrowser/ExampleEntries.h",
        "examples/ExampleBrowser/InProcessExampleBrowser.h",  # yes...
        "examples/SoftDemo/BunnyMesh.h",
        "examples/SoftDemo/SoftDemo.h",
        "examples/SoftDemo/TorusMesh.h",
        "examples/ExampleBrowser/OpenGLExampleBrowser.h",
        "examples/RenderingExamples/TinyRendererSetup.h",  # yes...
        "examples/SharedMemory/BodyJointInfoUtility.h",
        "examples/SharedMemory/GraphicsClientExample.h",
        "examples/SharedMemory/GraphicsServerExample.h",
        "examples/SharedMemory/GraphicsSharedMemoryBlock.h",
        "examples/SharedMemory/GraphicsSharedMemoryCommands.h",
        "examples/SharedMemory/GraphicsSharedMemoryPublic.h",
        "examples/ExampleBrowser/GL_ShapeDrawer.h",
        "examples/SharedMemory/InProcessMemory.h",
        "examples/SharedMemory/PhysicsClient.h",
        "examples/SharedMemory/PhysicsClientC_API.h",
        "examples/SharedMemory/PhysicsClientSharedMemory.h",
        "examples/SharedMemory/PhysicsClientSharedMemory2_C_API.h",
        "examples/SharedMemory/PhysicsClientSharedMemory_C_API.h",
        "examples/SharedMemory/PhysicsCommandProcessorInterface.h",
        "examples/SharedMemory/PhysicsDirect.h",
        "examples/SharedMemory/PhysicsDirectC_API.h",
        "examples/SharedMemory/PhysicsLoopBack.h",
        "examples/SharedMemory/PhysicsLoopBackC_API.h",
        "examples/SharedMemory/PhysicsServer.h",
        "examples/SharedMemory/PhysicsServerExample.h",
        "examples/SharedMemory/PhysicsServerExampleBullet2.h",
        "examples/SharedMemory/PhysicsServerSharedMemory.h",
        "examples/SharedMemory/PosixSharedMemory.h",
        "examples/SharedMemory/RemoteGUIHelper.h",
        "examples/SharedMemory/SharedMemoryBlock.h",
        "examples/SharedMemory/SharedMemoryCommandProcessor.h",
        "examples/SharedMemory/SharedMemoryCommands.h",
        "examples/SharedMemory/SharedMemoryCommon.h",
        "examples/SharedMemory/SharedMemoryInProcessPhysicsC_API.h",
        "examples/SharedMemory/SharedMemoryInterface.h",
        "examples/SharedMemory/SharedMemoryPublic.h",
        "examples/SharedMemory/SharedMemoryUserData.h",
        "examples/SharedMemory/Win32SharedMemory.h",
        "examples/SharedMemory/b3RobotSimulatorClientAPI_InternalData.h",
        "examples/SharedMemory/b3RobotSimulatorClientAPI_NoDirect.h",
        "examples/SharedMemory/b3RobotSimulatorClientAPI_NoGUI.h",
        "examples/SharedMemory/plugins/b3PluginCollisionInterface.h",
        # "examples/SharedMemory/IKTrajectoryHelper.h",
        # "examples/SharedMemory/PhysicsClientExample.h",
        # "examples/SharedMemory/PhysicsServerCommandProcessor.h",
    ],
    deps = [
        ":basic_demo",
        ":benchmarks",
        ":collision",
        ":common_interfaces",
        ":constraints",
        ":extras",
        ":forklift",
        ":gwen",
        ":gyroscopic_demo",
        ":heightfield",
        ":importers",
        ":multi_body",
        ":rigid_body",
        ":multi_threaded_demo",
        ":multi_threading",
        ":planar_2d",
        ":rendering_examples",
        ":src",
        ":tiny_renderer",
        ":utils",
        ":vehicles",
        ":voronoi_fracture",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "robot_simulator",

    srcs = glob([
        "examples/RobotSimulator/b3RobotSimulatorClientAPI.cpp",
    ]),

    hdrs = glob([
        "examples/RobotSimulator/b3RobotSimulatorClientAPI.h",
    ]),

    strip_include_prefix = "examples",
    deps = [
        ":collision",
        ":common_interfaces",
        ":extras",
        ":gwen",
        ":gwen_gui_support",
        ":shared_memory",
        ":src",
    ],
    defines = ["BT_USE_DOUBLE_PRECISION"],
    copts = bullet_copts,
)

cc_library(
    name = "bullet",
    deps = [
        ":robot_simulator",
        ":src",
    ],
    # linkstatic = 1,
)
