#include "gl_api.hpp"

#include <cassert>
#include <mutex>

namespace
{

// Null-initialize function pointers.

// GL_VERSION_1_0
PFNGLCULLFACEPROC mglCullFace = nullptr;
PFNGLFRONTFACEPROC mglFrontFace = nullptr;
PFNGLHINTPROC mglHint = nullptr;
PFNGLLINEWIDTHPROC mglLineWidth = nullptr;
PFNGLPOINTSIZEPROC mglPointSize = nullptr;
PFNGLPOLYGONMODEPROC mglPolygonMode = nullptr;
PFNGLSCISSORPROC mglScissor = nullptr;
PFNGLTEXPARAMETERFPROC mglTexParameterf = nullptr;
PFNGLTEXPARAMETERFVPROC mglTexParameterfv = nullptr;
PFNGLTEXPARAMETERIPROC mglTexParameteri = nullptr;
PFNGLTEXPARAMETERIVPROC mglTexParameteriv = nullptr;
PFNGLTEXIMAGE1DPROC mglTexImage1D = nullptr;
PFNGLTEXIMAGE2DPROC mglTexImage2D = nullptr;
PFNGLDRAWBUFFERPROC mglDrawBuffer = nullptr;
PFNGLCLEARPROC mglClear = nullptr;
PFNGLCLEARCOLORPROC mglClearColor = nullptr;
PFNGLCLEARSTENCILPROC mglClearStencil = nullptr;
PFNGLCLEARDEPTHPROC mglClearDepth = nullptr;
PFNGLSTENCILMASKPROC mglStencilMask = nullptr;
PFNGLCOLORMASKPROC mglColorMask = nullptr;
PFNGLDEPTHMASKPROC mglDepthMask = nullptr;
PFNGLDISABLEPROC mglDisable = nullptr;
PFNGLENABLEPROC mglEnable = nullptr;
PFNGLFINISHPROC mglFinish = nullptr;
PFNGLFLUSHPROC mglFlush = nullptr;
PFNGLBLENDFUNCPROC mglBlendFunc = nullptr;
PFNGLLOGICOPPROC mglLogicOp = nullptr;
PFNGLSTENCILFUNCPROC mglStencilFunc = nullptr;
PFNGLSTENCILOPPROC mglStencilOp = nullptr;
PFNGLDEPTHFUNCPROC mglDepthFunc = nullptr;
PFNGLPIXELSTOREFPROC mglPixelStoref = nullptr;
PFNGLPIXELSTOREIPROC mglPixelStorei = nullptr;
PFNGLREADBUFFERPROC mglReadBuffer = nullptr;
PFNGLREADPIXELSPROC mglReadPixels = nullptr;
PFNGLGETBOOLEANVPROC mglGetBooleanv = nullptr;
PFNGLGETDOUBLEVPROC mglGetDoublev = nullptr;
PFNGLGETERRORPROC mglGetError = nullptr;
PFNGLGETFLOATVPROC mglGetFloatv = nullptr;
PFNGLGETINTEGERVPROC mglGetIntegerv = nullptr;
PFNGLGETSTRINGPROC mglGetString = nullptr;
PFNGLGETTEXIMAGEPROC mglGetTexImage = nullptr;
PFNGLGETTEXPARAMETERFVPROC mglGetTexParameterfv = nullptr;
PFNGLGETTEXPARAMETERIVPROC mglGetTexParameteriv = nullptr;
PFNGLGETTEXLEVELPARAMETERFVPROC mglGetTexLevelParameterfv = nullptr;
PFNGLGETTEXLEVELPARAMETERIVPROC mglGetTexLevelParameteriv = nullptr;
PFNGLISENABLEDPROC mglIsEnabled = nullptr;
PFNGLDEPTHRANGEPROC mglDepthRange = nullptr;
PFNGLVIEWPORTPROC mglViewport = nullptr;

// GL_VERSION_1_1
PFNGLDRAWARRAYSPROC mglDrawArrays = nullptr;
PFNGLDRAWELEMENTSPROC mglDrawElements = nullptr;
PFNGLGETPOINTERVPROC mglGetPointerv = nullptr;
PFNGLPOLYGONOFFSETPROC mglPolygonOffset = nullptr;
PFNGLCOPYTEXIMAGE1DPROC mglCopyTexImage1D = nullptr;
PFNGLCOPYTEXIMAGE2DPROC mglCopyTexImage2D = nullptr;
PFNGLCOPYTEXSUBIMAGE1DPROC mglCopyTexSubImage1D = nullptr;
PFNGLCOPYTEXSUBIMAGE2DPROC mglCopyTexSubImage2D = nullptr;
PFNGLTEXSUBIMAGE1DPROC mglTexSubImage1D = nullptr;
PFNGLTEXSUBIMAGE2DPROC mglTexSubImage2D = nullptr;
PFNGLBINDTEXTUREPROC mglBindTexture = nullptr;
PFNGLDELETETEXTURESPROC mglDeleteTextures = nullptr;
PFNGLGENTEXTURESPROC mglGenTextures = nullptr;
PFNGLISTEXTUREPROC mglIsTexture = nullptr;

// GL_VERSION_1_2
PFNGLDRAWRANGEELEMENTSPROC mglDrawRangeElements = nullptr;
PFNGLTEXIMAGE3DPROC mglTexImage3D = nullptr;
PFNGLTEXSUBIMAGE3DPROC mglTexSubImage3D = nullptr;
PFNGLCOPYTEXSUBIMAGE3DPROC mglCopyTexSubImage3D = nullptr;

// GL_VERSION_1_3
PFNGLACTIVETEXTUREPROC mglActiveTexture = nullptr;
PFNGLSAMPLECOVERAGEPROC mglSampleCoverage = nullptr;
PFNGLCOMPRESSEDTEXIMAGE3DPROC mglCompressedTexImage3D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC mglCompressedTexImage2D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE1DPROC mglCompressedTexImage1D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC mglCompressedTexSubImage3D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC mglCompressedTexSubImage2D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC mglCompressedTexSubImage1D = nullptr;
PFNGLGETCOMPRESSEDTEXIMAGEPROC mglGetCompressedTexImage = nullptr;

// GL_VERSION_1_4
PFNGLBLENDFUNCSEPARATEPROC mglBlendFuncSeparate = nullptr;
PFNGLMULTIDRAWARRAYSPROC mglMultiDrawArrays = nullptr;
PFNGLMULTIDRAWELEMENTSPROC mglMultiDrawElements = nullptr;
PFNGLPOINTPARAMETERFPROC mglPointParameterf = nullptr;
PFNGLPOINTPARAMETERFVPROC mglPointParameterfv = nullptr;
PFNGLPOINTPARAMETERIPROC mglPointParameteri = nullptr;
PFNGLPOINTPARAMETERIVPROC mglPointParameteriv = nullptr;
PFNGLBLENDCOLORPROC mglBlendColor = nullptr;
PFNGLBLENDEQUATIONPROC mglBlendEquation = nullptr;

// GL_VERSION_1_5
PFNGLGENQUERIESPROC mglGenQueries = nullptr;
PFNGLDELETEQUERIESPROC mglDeleteQueries = nullptr;
PFNGLISQUERYPROC mglIsQuery = nullptr;
PFNGLBEGINQUERYPROC mglBeginQuery = nullptr;
PFNGLENDQUERYPROC mglEndQuery = nullptr;
PFNGLGETQUERYIVPROC mglGetQueryiv = nullptr;
PFNGLGETQUERYOBJECTIVPROC mglGetQueryObjectiv = nullptr;
PFNGLGETQUERYOBJECTUIVPROC mglGetQueryObjectuiv = nullptr;
PFNGLBINDBUFFERPROC mglBindBuffer = nullptr;
PFNGLDELETEBUFFERSPROC mglDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC mglGenBuffers = nullptr;
PFNGLISBUFFERPROC mglIsBuffer = nullptr;
PFNGLBUFFERDATAPROC mglBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC mglBufferSubData = nullptr;
PFNGLGETBUFFERSUBDATAPROC mglGetBufferSubData = nullptr;
PFNGLMAPBUFFERPROC mglMapBuffer = nullptr;
PFNGLUNMAPBUFFERPROC mglUnmapBuffer = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC mglGetBufferParameteriv = nullptr;
PFNGLGETBUFFERPOINTERVPROC mglGetBufferPointerv = nullptr;

// GL_VERSION_2_0
PFNGLBLENDEQUATIONSEPARATEPROC mglBlendEquationSeparate = nullptr;
PFNGLDRAWBUFFERSPROC mglDrawBuffers = nullptr;
PFNGLSTENCILOPSEPARATEPROC mglStencilOpSeparate = nullptr;
PFNGLSTENCILFUNCSEPARATEPROC mglStencilFuncSeparate = nullptr;
PFNGLSTENCILMASKSEPARATEPROC mglStencilMaskSeparate = nullptr;
PFNGLATTACHSHADERPROC mglAttachShader = nullptr;
PFNGLBINDATTRIBLOCATIONPROC mglBindAttribLocation = nullptr;
PFNGLCOMPILESHADERPROC mglCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC mglCreateProgram = nullptr;
PFNGLCREATESHADERPROC mglCreateShader = nullptr;
PFNGLDELETEPROGRAMPROC mglDeleteProgram = nullptr;
PFNGLDELETESHADERPROC mglDeleteShader = nullptr;
PFNGLDETACHSHADERPROC mglDetachShader = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC mglDisableVertexAttribArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC mglEnableVertexAttribArray = nullptr;
PFNGLGETACTIVEATTRIBPROC mglGetActiveAttrib = nullptr;
PFNGLGETACTIVEUNIFORMPROC mglGetActiveUniform = nullptr;
PFNGLGETATTACHEDSHADERSPROC mglGetAttachedShaders = nullptr;
PFNGLGETATTRIBLOCATIONPROC mglGetAttribLocation = nullptr;
PFNGLGETPROGRAMIVPROC mglGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC mglGetProgramInfoLog = nullptr;
PFNGLGETSHADERIVPROC mglGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC mglGetShaderInfoLog = nullptr;
PFNGLGETSHADERSOURCEPROC mglGetShaderSource = nullptr;
PFNGLGETUNIFORMLOCATIONPROC mglGetUniformLocation = nullptr;
PFNGLGETUNIFORMFVPROC mglGetUniformfv = nullptr;
PFNGLGETUNIFORMIVPROC mglGetUniformiv = nullptr;
PFNGLGETVERTEXATTRIBDVPROC mglGetVertexAttribdv = nullptr;
PFNGLGETVERTEXATTRIBFVPROC mglGetVertexAttribfv = nullptr;
PFNGLGETVERTEXATTRIBIVPROC mglGetVertexAttribiv = nullptr;
PFNGLGETVERTEXATTRIBPOINTERVPROC mglGetVertexAttribPointerv = nullptr;
PFNGLISPROGRAMPROC mglIsProgram = nullptr;
PFNGLISSHADERPROC mglIsShader = nullptr;
PFNGLLINKPROGRAMPROC mglLinkProgram = nullptr;
PFNGLSHADERSOURCEPROC mglShaderSource = nullptr;
PFNGLUSEPROGRAMPROC mglUseProgram = nullptr;
PFNGLUNIFORM1FPROC mglUniform1f = nullptr;
PFNGLUNIFORM2FPROC mglUniform2f = nullptr;
PFNGLUNIFORM3FPROC mglUniform3f = nullptr;
PFNGLUNIFORM4FPROC mglUniform4f = nullptr;
PFNGLUNIFORM1IPROC mglUniform1i = nullptr;
PFNGLUNIFORM2IPROC mglUniform2i = nullptr;
PFNGLUNIFORM3IPROC mglUniform3i = nullptr;
PFNGLUNIFORM4IPROC mglUniform4i = nullptr;
PFNGLUNIFORM1FVPROC mglUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC mglUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC mglUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC mglUniform4fv = nullptr;
PFNGLUNIFORM1IVPROC mglUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC mglUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC mglUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC mglUniform4iv = nullptr;
PFNGLUNIFORMMATRIX2FVPROC mglUniformMatrix2fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC mglUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC mglUniformMatrix4fv = nullptr;
PFNGLVALIDATEPROGRAMPROC mglValidateProgram = nullptr;
PFNGLVERTEXATTRIB1DPROC mglVertexAttrib1d = nullptr;
PFNGLVERTEXATTRIB1DVPROC mglVertexAttrib1dv = nullptr;
PFNGLVERTEXATTRIB1FPROC mglVertexAttrib1f = nullptr;
PFNGLVERTEXATTRIB1FVPROC mglVertexAttrib1fv = nullptr;
PFNGLVERTEXATTRIB1SPROC mglVertexAttrib1s = nullptr;
PFNGLVERTEXATTRIB1SVPROC mglVertexAttrib1sv = nullptr;
PFNGLVERTEXATTRIB2DPROC mglVertexAttrib2d = nullptr;
PFNGLVERTEXATTRIB2DVPROC mglVertexAttrib2dv = nullptr;
PFNGLVERTEXATTRIB2FPROC mglVertexAttrib2f = nullptr;
PFNGLVERTEXATTRIB2FVPROC mglVertexAttrib2fv = nullptr;
PFNGLVERTEXATTRIB2SPROC mglVertexAttrib2s = nullptr;
PFNGLVERTEXATTRIB2SVPROC mglVertexAttrib2sv = nullptr;
PFNGLVERTEXATTRIB3DPROC mglVertexAttrib3d = nullptr;
PFNGLVERTEXATTRIB3DVPROC mglVertexAttrib3dv = nullptr;
PFNGLVERTEXATTRIB3FPROC mglVertexAttrib3f = nullptr;
PFNGLVERTEXATTRIB3FVPROC mglVertexAttrib3fv = nullptr;
PFNGLVERTEXATTRIB3SPROC mglVertexAttrib3s = nullptr;
PFNGLVERTEXATTRIB3SVPROC mglVertexAttrib3sv = nullptr;
PFNGLVERTEXATTRIB4NBVPROC mglVertexAttrib4Nbv = nullptr;
PFNGLVERTEXATTRIB4NIVPROC mglVertexAttrib4Niv = nullptr;
PFNGLVERTEXATTRIB4NSVPROC mglVertexAttrib4Nsv = nullptr;
PFNGLVERTEXATTRIB4NUBPROC mglVertexAttrib4Nub = nullptr;
PFNGLVERTEXATTRIB4NUBVPROC mglVertexAttrib4Nubv = nullptr;
PFNGLVERTEXATTRIB4NUIVPROC mglVertexAttrib4Nuiv = nullptr;
PFNGLVERTEXATTRIB4NUSVPROC mglVertexAttrib4Nusv = nullptr;
PFNGLVERTEXATTRIB4BVPROC mglVertexAttrib4bv = nullptr;
PFNGLVERTEXATTRIB4DPROC mglVertexAttrib4d = nullptr;
PFNGLVERTEXATTRIB4DVPROC mglVertexAttrib4dv = nullptr;
PFNGLVERTEXATTRIB4FPROC mglVertexAttrib4f = nullptr;
PFNGLVERTEXATTRIB4FVPROC mglVertexAttrib4fv = nullptr;
PFNGLVERTEXATTRIB4IVPROC mglVertexAttrib4iv = nullptr;
PFNGLVERTEXATTRIB4SPROC mglVertexAttrib4s = nullptr;
PFNGLVERTEXATTRIB4SVPROC mglVertexAttrib4sv = nullptr;
PFNGLVERTEXATTRIB4UBVPROC mglVertexAttrib4ubv = nullptr;
PFNGLVERTEXATTRIB4UIVPROC mglVertexAttrib4uiv = nullptr;
PFNGLVERTEXATTRIB4USVPROC mglVertexAttrib4usv = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC mglVertexAttribPointer = nullptr;

// GL_VERSION_2_1
PFNGLUNIFORMMATRIX2X3FVPROC mglUniformMatrix2x3fv = nullptr;
PFNGLUNIFORMMATRIX3X2FVPROC mglUniformMatrix3x2fv = nullptr;
PFNGLUNIFORMMATRIX2X4FVPROC mglUniformMatrix2x4fv = nullptr;
PFNGLUNIFORMMATRIX4X2FVPROC mglUniformMatrix4x2fv = nullptr;
PFNGLUNIFORMMATRIX3X4FVPROC mglUniformMatrix3x4fv = nullptr;
PFNGLUNIFORMMATRIX4X3FVPROC mglUniformMatrix4x3fv = nullptr;

// GL_VERSION_3_0
PFNGLCOLORMASKIPROC mglColorMaski = nullptr;
PFNGLGETBOOLEANI_VPROC mglGetBooleani_v = nullptr;
PFNGLGETINTEGERI_VPROC mglGetIntegeri_v = nullptr;
PFNGLENABLEIPROC mglEnablei = nullptr;
PFNGLDISABLEIPROC mglDisablei = nullptr;
PFNGLISENABLEDIPROC mglIsEnabledi = nullptr;
PFNGLBEGINTRANSFORMFEEDBACKPROC mglBeginTransformFeedback = nullptr;
PFNGLENDTRANSFORMFEEDBACKPROC mglEndTransformFeedback = nullptr;
PFNGLBINDBUFFERRANGEPROC mglBindBufferRange = nullptr;
PFNGLBINDBUFFERBASEPROC mglBindBufferBase = nullptr;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC mglTransformFeedbackVaryings = nullptr;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC mglGetTransformFeedbackVarying = nullptr;
PFNGLCLAMPCOLORPROC mglClampColor = nullptr;
PFNGLBEGINCONDITIONALRENDERPROC mglBeginConditionalRender = nullptr;
PFNGLENDCONDITIONALRENDERPROC mglEndConditionalRender = nullptr;
PFNGLVERTEXATTRIBIPOINTERPROC mglVertexAttribIPointer = nullptr;
PFNGLGETVERTEXATTRIBIIVPROC mglGetVertexAttribIiv = nullptr;
PFNGLGETVERTEXATTRIBIUIVPROC mglGetVertexAttribIuiv = nullptr;
PFNGLVERTEXATTRIBI1IPROC mglVertexAttribI1i = nullptr;
PFNGLVERTEXATTRIBI2IPROC mglVertexAttribI2i = nullptr;
PFNGLVERTEXATTRIBI3IPROC mglVertexAttribI3i = nullptr;
PFNGLVERTEXATTRIBI4IPROC mglVertexAttribI4i = nullptr;
PFNGLVERTEXATTRIBI1UIPROC mglVertexAttribI1ui = nullptr;
PFNGLVERTEXATTRIBI2UIPROC mglVertexAttribI2ui = nullptr;
PFNGLVERTEXATTRIBI3UIPROC mglVertexAttribI3ui = nullptr;
PFNGLVERTEXATTRIBI4UIPROC mglVertexAttribI4ui = nullptr;
PFNGLVERTEXATTRIBI1IVPROC mglVertexAttribI1iv = nullptr;
PFNGLVERTEXATTRIBI2IVPROC mglVertexAttribI2iv = nullptr;
PFNGLVERTEXATTRIBI3IVPROC mglVertexAttribI3iv = nullptr;
PFNGLVERTEXATTRIBI4IVPROC mglVertexAttribI4iv = nullptr;
PFNGLVERTEXATTRIBI1UIVPROC mglVertexAttribI1uiv = nullptr;
PFNGLVERTEXATTRIBI2UIVPROC mglVertexAttribI2uiv = nullptr;
PFNGLVERTEXATTRIBI3UIVPROC mglVertexAttribI3uiv = nullptr;
PFNGLVERTEXATTRIBI4UIVPROC mglVertexAttribI4uiv = nullptr;
PFNGLVERTEXATTRIBI4BVPROC mglVertexAttribI4bv = nullptr;
PFNGLVERTEXATTRIBI4SVPROC mglVertexAttribI4sv = nullptr;
PFNGLVERTEXATTRIBI4UBVPROC mglVertexAttribI4ubv = nullptr;
PFNGLVERTEXATTRIBI4USVPROC mglVertexAttribI4usv = nullptr;
PFNGLGETUNIFORMUIVPROC mglGetUniformuiv = nullptr;
PFNGLBINDFRAGDATALOCATIONPROC mglBindFragDataLocation = nullptr;
PFNGLGETFRAGDATALOCATIONPROC mglGetFragDataLocation = nullptr;
PFNGLUNIFORM1UIPROC mglUniform1ui = nullptr;
PFNGLUNIFORM2UIPROC mglUniform2ui = nullptr;
PFNGLUNIFORM3UIPROC mglUniform3ui = nullptr;
PFNGLUNIFORM4UIPROC mglUniform4ui = nullptr;
PFNGLUNIFORM1UIVPROC mglUniform1uiv = nullptr;
PFNGLUNIFORM2UIVPROC mglUniform2uiv = nullptr;
PFNGLUNIFORM3UIVPROC mglUniform3uiv = nullptr;
PFNGLUNIFORM4UIVPROC mglUniform4uiv = nullptr;
PFNGLTEXPARAMETERIIVPROC mglTexParameterIiv = nullptr;
PFNGLTEXPARAMETERIUIVPROC mglTexParameterIuiv = nullptr;
PFNGLGETTEXPARAMETERIIVPROC mglGetTexParameterIiv = nullptr;
PFNGLGETTEXPARAMETERIUIVPROC mglGetTexParameterIuiv = nullptr;
PFNGLCLEARBUFFERIVPROC mglClearBufferiv = nullptr;
PFNGLCLEARBUFFERUIVPROC mglClearBufferuiv = nullptr;
PFNGLCLEARBUFFERFVPROC mglClearBufferfv = nullptr;
PFNGLCLEARBUFFERFIPROC mglClearBufferfi = nullptr;
PFNGLGETSTRINGIPROC mglGetStringi = nullptr;
PFNGLISRENDERBUFFERPROC mglIsRenderbuffer = nullptr;
PFNGLBINDRENDERBUFFERPROC mglBindRenderbuffer = nullptr;
PFNGLDELETERENDERBUFFERSPROC mglDeleteRenderbuffers = nullptr;
PFNGLGENRENDERBUFFERSPROC mglGenRenderbuffers = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC mglRenderbufferStorage = nullptr;
PFNGLGETRENDERBUFFERPARAMETERIVPROC mglGetRenderbufferParameteriv = nullptr;
PFNGLISFRAMEBUFFERPROC mglIsFramebuffer = nullptr;
PFNGLBINDFRAMEBUFFERPROC mglBindFramebuffer = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC mglDeleteFramebuffers = nullptr;
PFNGLGENFRAMEBUFFERSPROC mglGenFramebuffers = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC mglCheckFramebufferStatus = nullptr;
PFNGLFRAMEBUFFERTEXTURE1DPROC mglFramebufferTexture1D = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC mglFramebufferTexture2D = nullptr;
PFNGLFRAMEBUFFERTEXTURE3DPROC mglFramebufferTexture3D = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC mglFramebufferRenderbuffer = nullptr;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC mglGetFramebufferAttachmentParameteriv = nullptr;
PFNGLGENERATEMIPMAPPROC mglGenerateMipmap = nullptr;
PFNGLBLITFRAMEBUFFERPROC mglBlitFramebuffer = nullptr;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC mglRenderbufferStorageMultisample = nullptr;
PFNGLFRAMEBUFFERTEXTURELAYERPROC mglFramebufferTextureLayer = nullptr;
PFNGLMAPBUFFERRANGEPROC mglMapBufferRange = nullptr;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC mglFlushMappedBufferRange = nullptr;
PFNGLBINDVERTEXARRAYPROC mglBindVertexArray = nullptr;
PFNGLDELETEVERTEXARRAYSPROC mglDeleteVertexArrays = nullptr;
PFNGLGENVERTEXARRAYSPROC mglGenVertexArrays = nullptr;
PFNGLISVERTEXARRAYPROC mglIsVertexArray = nullptr;

// GL_VERSION_3_1
PFNGLDRAWARRAYSINSTANCEDPROC mglDrawArraysInstanced = nullptr;
PFNGLDRAWELEMENTSINSTANCEDPROC mglDrawElementsInstanced = nullptr;
PFNGLTEXBUFFERPROC mglTexBuffer = nullptr;
PFNGLPRIMITIVERESTARTINDEXPROC mglPrimitiveRestartIndex = nullptr;
PFNGLCOPYBUFFERSUBDATAPROC mglCopyBufferSubData = nullptr;
PFNGLGETUNIFORMINDICESPROC mglGetUniformIndices = nullptr;
PFNGLGETACTIVEUNIFORMSIVPROC mglGetActiveUniformsiv = nullptr;
PFNGLGETACTIVEUNIFORMNAMEPROC mglGetActiveUniformName = nullptr;
PFNGLGETUNIFORMBLOCKINDEXPROC mglGetUniformBlockIndex = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC mglGetActiveUniformBlockiv = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC mglGetActiveUniformBlockName = nullptr;
PFNGLUNIFORMBLOCKBINDINGPROC mglUniformBlockBinding = nullptr;

// GL_VERSION_3_2
PFNGLDRAWELEMENTSBASEVERTEXPROC mglDrawElementsBaseVertex = nullptr;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC mglDrawRangeElementsBaseVertex = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC mglDrawElementsInstancedBaseVertex = nullptr;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC mglMultiDrawElementsBaseVertex = nullptr;
PFNGLPROVOKINGVERTEXPROC mglProvokingVertex = nullptr;
PFNGLFENCESYNCPROC mglFenceSync = nullptr;
PFNGLISSYNCPROC mglIsSync = nullptr;
PFNGLDELETESYNCPROC mglDeleteSync = nullptr;
PFNGLCLIENTWAITSYNCPROC mglClientWaitSync = nullptr;
PFNGLWAITSYNCPROC mglWaitSync = nullptr;
PFNGLGETINTEGER64VPROC mglGetInteger64v = nullptr;
PFNGLGETSYNCIVPROC mglGetSynciv = nullptr;
PFNGLGETINTEGER64I_VPROC mglGetInteger64i_v = nullptr;
PFNGLGETBUFFERPARAMETERI64VPROC mglGetBufferParameteri64v = nullptr;
PFNGLFRAMEBUFFERTEXTUREPROC mglFramebufferTexture = nullptr;
PFNGLTEXIMAGE2DMULTISAMPLEPROC mglTexImage2DMultisample = nullptr;
PFNGLTEXIMAGE3DMULTISAMPLEPROC mglTexImage3DMultisample = nullptr;
PFNGLGETMULTISAMPLEFVPROC mglGetMultisamplefv = nullptr;
PFNGLSAMPLEMASKIPROC mglSampleMaski = nullptr;

// GL_VERSION_3_3
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC mglBindFragDataLocationIndexed = nullptr;
PFNGLGETFRAGDATAINDEXPROC mglGetFragDataIndex = nullptr;
PFNGLGENSAMPLERSPROC mglGenSamplers = nullptr;
PFNGLDELETESAMPLERSPROC mglDeleteSamplers = nullptr;
PFNGLISSAMPLERPROC mglIsSampler = nullptr;
PFNGLBINDSAMPLERPROC mglBindSampler = nullptr;
PFNGLSAMPLERPARAMETERIPROC mglSamplerParameteri = nullptr;
PFNGLSAMPLERPARAMETERIVPROC mglSamplerParameteriv = nullptr;
PFNGLSAMPLERPARAMETERFPROC mglSamplerParameterf = nullptr;
PFNGLSAMPLERPARAMETERFVPROC mglSamplerParameterfv = nullptr;
PFNGLSAMPLERPARAMETERIIVPROC mglSamplerParameterIiv = nullptr;
PFNGLSAMPLERPARAMETERIUIVPROC mglSamplerParameterIuiv = nullptr;
PFNGLGETSAMPLERPARAMETERIVPROC mglGetSamplerParameteriv = nullptr;
PFNGLGETSAMPLERPARAMETERIIVPROC mglGetSamplerParameterIiv = nullptr;
PFNGLGETSAMPLERPARAMETERFVPROC mglGetSamplerParameterfv = nullptr;
PFNGLGETSAMPLERPARAMETERIUIVPROC mglGetSamplerParameterIuiv = nullptr;
PFNGLQUERYCOUNTERPROC mglQueryCounter = nullptr;
PFNGLGETQUERYOBJECTI64VPROC mglGetQueryObjecti64v = nullptr;
PFNGLGETQUERYOBJECTUI64VPROC mglGetQueryObjectui64v = nullptr;
PFNGLVERTEXATTRIBDIVISORPROC mglVertexAttribDivisor = nullptr;
PFNGLVERTEXATTRIBP1UIPROC mglVertexAttribP1ui = nullptr;
PFNGLVERTEXATTRIBP1UIVPROC mglVertexAttribP1uiv = nullptr;
PFNGLVERTEXATTRIBP2UIPROC mglVertexAttribP2ui = nullptr;
PFNGLVERTEXATTRIBP2UIVPROC mglVertexAttribP2uiv = nullptr;
PFNGLVERTEXATTRIBP3UIPROC mglVertexAttribP3ui = nullptr;
PFNGLVERTEXATTRIBP3UIVPROC mglVertexAttribP3uiv = nullptr;
PFNGLVERTEXATTRIBP4UIPROC mglVertexAttribP4ui = nullptr;
PFNGLVERTEXATTRIBP4UIVPROC mglVertexAttribP4uiv = nullptr;

// GL_ARB_debug_output
PFNGLDEBUGMESSAGECONTROLARBPROC mglDebugMessageControlARB = nullptr;
PFNGLDEBUGMESSAGEINSERTARBPROC mglDebugMessageInsertARB = nullptr;
PFNGLDEBUGMESSAGECALLBACKARBPROC mglDebugMessageCallbackARB = nullptr;
PFNGLGETDEBUGMESSAGELOGARBPROC mglGetDebugMessageLogARB = nullptr;


void import_impl()
{
    // Load GL function pointers with GLFW.

    // GL_VERSION_1_0
    mglCullFace = reinterpret_cast<PFNGLCULLFACEPROC>(glfwGetProcAddress("glCullFace"));
    assert(mglCullFace && "glCullFace() import failed.");
    mglFrontFace = reinterpret_cast<PFNGLFRONTFACEPROC>(glfwGetProcAddress("glFrontFace"));
    assert(mglFrontFace && "glFrontFace() import failed.");
    mglHint = reinterpret_cast<PFNGLHINTPROC>(glfwGetProcAddress("glHint"));
    assert(mglHint && "glHint() import failed.");
    mglLineWidth = reinterpret_cast<PFNGLLINEWIDTHPROC>(glfwGetProcAddress("glLineWidth"));
    assert(mglLineWidth && "glLineWidth() import failed.");
    mglPointSize = reinterpret_cast<PFNGLPOINTSIZEPROC>(glfwGetProcAddress("glPointSize"));
    assert(mglPointSize && "glPointSize() import failed.");
    mglPolygonMode = reinterpret_cast<PFNGLPOLYGONMODEPROC>(glfwGetProcAddress("glPolygonMode"));
    assert(mglPolygonMode && "glPolygonMode() import failed.");
    mglScissor = reinterpret_cast<PFNGLSCISSORPROC>(glfwGetProcAddress("glScissor"));
    assert(mglScissor && "glScissor() import failed.");
    mglTexParameterf = reinterpret_cast<PFNGLTEXPARAMETERFPROC>(glfwGetProcAddress("glTexParameterf"));
    assert(mglTexParameterf && "glTexParameterf() import failed.");
    mglTexParameterfv = reinterpret_cast<PFNGLTEXPARAMETERFVPROC>(glfwGetProcAddress("glTexParameterfv"));
    assert(mglTexParameterfv && "glTexParameterfv() import failed.");
    mglTexParameteri = reinterpret_cast<PFNGLTEXPARAMETERIPROC>(glfwGetProcAddress("glTexParameteri"));
    assert(mglTexParameteri && "glTexParameteri() import failed.");
    mglTexParameteriv = reinterpret_cast<PFNGLTEXPARAMETERIVPROC>(glfwGetProcAddress("glTexParameteriv"));
    assert(mglTexParameteriv && "glTexParameteriv() import failed.");
    mglTexImage1D = reinterpret_cast<PFNGLTEXIMAGE1DPROC>(glfwGetProcAddress("glTexImage1D"));
    assert(mglTexImage1D && "glTexImage1D() import failed.");
    mglTexImage2D = reinterpret_cast<PFNGLTEXIMAGE2DPROC>(glfwGetProcAddress("glTexImage2D"));
    assert(mglTexImage2D && "glTexImage2D() import failed.");
    mglDrawBuffer = reinterpret_cast<PFNGLDRAWBUFFERPROC>(glfwGetProcAddress("glDrawBuffer"));
    assert(mglDrawBuffer && "glDrawBuffer() import failed.");
    mglClear = reinterpret_cast<PFNGLCLEARPROC>(glfwGetProcAddress("glClear"));
    assert(mglClear && "glClear() import failed.");
    mglClearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>(glfwGetProcAddress("glClearColor"));
    assert(mglClearColor && "glClearColor() import failed.");
    mglClearStencil = reinterpret_cast<PFNGLCLEARSTENCILPROC>(glfwGetProcAddress("glClearStencil"));
    assert(mglClearStencil && "glClearStencil() import failed.");
    mglClearDepth = reinterpret_cast<PFNGLCLEARDEPTHPROC>(glfwGetProcAddress("glClearDepth"));
    assert(mglClearDepth && "glClearDepth() import failed.");
    mglStencilMask = reinterpret_cast<PFNGLSTENCILMASKPROC>(glfwGetProcAddress("glStencilMask"));
    assert(mglStencilMask && "glStencilMask() import failed.");
    mglColorMask = reinterpret_cast<PFNGLCOLORMASKPROC>(glfwGetProcAddress("glColorMask"));
    assert(mglColorMask && "glColorMask() import failed.");
    mglDepthMask = reinterpret_cast<PFNGLDEPTHMASKPROC>(glfwGetProcAddress("glDepthMask"));
    assert(mglDepthMask && "glDepthMask() import failed.");
    mglDisable = reinterpret_cast<PFNGLDISABLEPROC>(glfwGetProcAddress("glDisable"));
    assert(mglDisable && "glDisable() import failed.");
    mglEnable = reinterpret_cast<PFNGLENABLEPROC>(glfwGetProcAddress("glEnable"));
    assert(mglEnable && "glEnable() import failed.");
    mglFinish = reinterpret_cast<PFNGLFINISHPROC>(glfwGetProcAddress("glFinish"));
    assert(mglFinish && "glFinish() import failed.");
    mglFlush = reinterpret_cast<PFNGLFLUSHPROC>(glfwGetProcAddress("glFlush"));
    assert(mglFlush && "glFlush() import failed.");
    mglBlendFunc = reinterpret_cast<PFNGLBLENDFUNCPROC>(glfwGetProcAddress("glBlendFunc"));
    assert(mglBlendFunc && "glBlendFunc() import failed.");
    mglLogicOp = reinterpret_cast<PFNGLLOGICOPPROC>(glfwGetProcAddress("glLogicOp"));
    assert(mglLogicOp && "glLogicOp() import failed.");
    mglStencilFunc = reinterpret_cast<PFNGLSTENCILFUNCPROC>(glfwGetProcAddress("glStencilFunc"));
    assert(mglStencilFunc && "glStencilFunc() import failed.");
    mglStencilOp = reinterpret_cast<PFNGLSTENCILOPPROC>(glfwGetProcAddress("glStencilOp"));
    assert(mglStencilOp && "glStencilOp() import failed.");
    mglDepthFunc = reinterpret_cast<PFNGLDEPTHFUNCPROC>(glfwGetProcAddress("glDepthFunc"));
    assert(mglDepthFunc && "glDepthFunc() import failed.");
    mglPixelStoref = reinterpret_cast<PFNGLPIXELSTOREFPROC>(glfwGetProcAddress("glPixelStoref"));
    assert(mglPixelStoref && "glPixelStoref() import failed.");
    mglPixelStorei = reinterpret_cast<PFNGLPIXELSTOREIPROC>(glfwGetProcAddress("glPixelStorei"));
    assert(mglPixelStorei && "glPixelStorei() import failed.");
    mglReadBuffer = reinterpret_cast<PFNGLREADBUFFERPROC>(glfwGetProcAddress("glReadBuffer"));
    assert(mglReadBuffer && "glReadBuffer() import failed.");
    mglReadPixels = reinterpret_cast<PFNGLREADPIXELSPROC>(glfwGetProcAddress("glReadPixels"));
    assert(mglReadPixels && "glReadPixels() import failed.");
    mglGetBooleanv = reinterpret_cast<PFNGLGETBOOLEANVPROC>(glfwGetProcAddress("glGetBooleanv"));
    assert(mglGetBooleanv && "glGetBooleanv() import failed.");
    mglGetDoublev = reinterpret_cast<PFNGLGETDOUBLEVPROC>(glfwGetProcAddress("glGetDoublev"));
    assert(mglGetDoublev && "glGetDoublev() import failed.");
    mglGetError = reinterpret_cast<PFNGLGETERRORPROC>(glfwGetProcAddress("glGetError"));
    assert(mglGetError && "glGetError() import failed.");
    mglGetFloatv = reinterpret_cast<PFNGLGETFLOATVPROC>(glfwGetProcAddress("glGetFloatv"));
    assert(mglGetFloatv && "glGetFloatv() import failed.");
    mglGetIntegerv = reinterpret_cast<PFNGLGETINTEGERVPROC>(glfwGetProcAddress("glGetIntegerv"));
    assert(mglGetIntegerv && "glGetIntegerv() import failed.");
    mglGetString = reinterpret_cast<PFNGLGETSTRINGPROC>(glfwGetProcAddress("glGetString"));
    assert(mglGetString && "glGetString() import failed.");
    mglGetTexImage = reinterpret_cast<PFNGLGETTEXIMAGEPROC>(glfwGetProcAddress("glGetTexImage"));
    assert(mglGetTexImage && "glGetTexImage() import failed.");
    mglGetTexParameterfv = reinterpret_cast<PFNGLGETTEXPARAMETERFVPROC>(glfwGetProcAddress("glGetTexParameterfv"));
    assert(mglGetTexParameterfv && "glGetTexParameterfv() import failed.");
    mglGetTexParameteriv = reinterpret_cast<PFNGLGETTEXPARAMETERIVPROC>(glfwGetProcAddress("glGetTexParameteriv"));
    assert(mglGetTexParameteriv && "glGetTexParameteriv() import failed.");
    mglGetTexLevelParameterfv = reinterpret_cast<PFNGLGETTEXLEVELPARAMETERFVPROC>(glfwGetProcAddress("glGetTexLevelParameterfv"));
    assert(mglGetTexLevelParameterfv && "glGetTexLevelParameterfv() import failed.");
    mglGetTexLevelParameteriv = reinterpret_cast<PFNGLGETTEXLEVELPARAMETERIVPROC>(glfwGetProcAddress("glGetTexLevelParameteriv"));
    assert(mglGetTexLevelParameteriv && "glGetTexLevelParameteriv() import failed.");
    mglIsEnabled = reinterpret_cast<PFNGLISENABLEDPROC>(glfwGetProcAddress("glIsEnabled"));
    assert(mglIsEnabled && "glIsEnabled() import failed.");
    mglDepthRange = reinterpret_cast<PFNGLDEPTHRANGEPROC>(glfwGetProcAddress("glDepthRange"));
    assert(mglDepthRange && "glDepthRange() import failed.");
    mglViewport = reinterpret_cast<PFNGLVIEWPORTPROC>(glfwGetProcAddress("glViewport"));
    assert(mglViewport && "glViewport() import failed.");

    // GL_VERSION_1_1
    mglDrawArrays = reinterpret_cast<PFNGLDRAWARRAYSPROC>(glfwGetProcAddress("glDrawArrays"));
    assert(mglDrawArrays && "glDrawArrays() import failed.");
    mglDrawElements = reinterpret_cast<PFNGLDRAWELEMENTSPROC>(glfwGetProcAddress("glDrawElements"));
    assert(mglDrawElements && "glDrawElements() import failed.");
    mglGetPointerv = reinterpret_cast<PFNGLGETPOINTERVPROC>(glfwGetProcAddress("glGetPointerv"));
    assert(mglGetPointerv && "glGetPointerv() import failed.");
    mglPolygonOffset = reinterpret_cast<PFNGLPOLYGONOFFSETPROC>(glfwGetProcAddress("glPolygonOffset"));
    assert(mglPolygonOffset && "glPolygonOffset() import failed.");
    mglCopyTexImage1D = reinterpret_cast<PFNGLCOPYTEXIMAGE1DPROC>(glfwGetProcAddress("glCopyTexImage1D"));
    assert(mglCopyTexImage1D && "glCopyTexImage1D() import failed.");
    mglCopyTexImage2D = reinterpret_cast<PFNGLCOPYTEXIMAGE2DPROC>(glfwGetProcAddress("glCopyTexImage2D"));
    assert(mglCopyTexImage2D && "glCopyTexImage2D() import failed.");
    mglCopyTexSubImage1D = reinterpret_cast<PFNGLCOPYTEXSUBIMAGE1DPROC>(glfwGetProcAddress("glCopyTexSubImage1D"));
    assert(mglCopyTexSubImage1D && "glCopyTexSubImage1D() import failed.");
    mglCopyTexSubImage2D = reinterpret_cast<PFNGLCOPYTEXSUBIMAGE2DPROC>(glfwGetProcAddress("glCopyTexSubImage2D"));
    assert(mglCopyTexSubImage2D && "glCopyTexSubImage2D() import failed.");
    mglTexSubImage1D = reinterpret_cast<PFNGLTEXSUBIMAGE1DPROC>(glfwGetProcAddress("glTexSubImage1D"));
    assert(mglTexSubImage1D && "glTexSubImage1D() import failed.");
    mglTexSubImage2D = reinterpret_cast<PFNGLTEXSUBIMAGE2DPROC>(glfwGetProcAddress("glTexSubImage2D"));
    assert(mglTexSubImage2D && "glTexSubImage2D() import failed.");
    mglBindTexture = reinterpret_cast<PFNGLBINDTEXTUREPROC>(glfwGetProcAddress("glBindTexture"));
    assert(mglBindTexture && "glBindTexture() import failed.");
    mglDeleteTextures = reinterpret_cast<PFNGLDELETETEXTURESPROC>(glfwGetProcAddress("glDeleteTextures"));
    assert(mglDeleteTextures && "glDeleteTextures() import failed.");
    mglGenTextures = reinterpret_cast<PFNGLGENTEXTURESPROC>(glfwGetProcAddress("glGenTextures"));
    assert(mglGenTextures && "glGenTextures() import failed.");
    mglIsTexture = reinterpret_cast<PFNGLISTEXTUREPROC>(glfwGetProcAddress("glIsTexture"));
    assert(mglIsTexture && "glIsTexture() import failed.");

    // GL_VERSION_1_2
    mglDrawRangeElements = reinterpret_cast<PFNGLDRAWRANGEELEMENTSPROC>(glfwGetProcAddress("glDrawRangeElements"));
    assert(mglDrawRangeElements && "glDrawRangeElements() import failed.");
    mglTexImage3D = reinterpret_cast<PFNGLTEXIMAGE3DPROC>(glfwGetProcAddress("glTexImage3D"));
    assert(mglTexImage3D && "glTexImage3D() import failed.");
    mglTexSubImage3D = reinterpret_cast<PFNGLTEXSUBIMAGE3DPROC>(glfwGetProcAddress("glTexSubImage3D"));
    assert(mglTexSubImage3D && "glTexSubImage3D() import failed.");
    mglCopyTexSubImage3D = reinterpret_cast<PFNGLCOPYTEXSUBIMAGE3DPROC>(glfwGetProcAddress("glCopyTexSubImage3D"));
    assert(mglCopyTexSubImage3D && "glCopyTexSubImage3D() import failed.");

    // GL_VERSION_1_3
    mglActiveTexture = reinterpret_cast<PFNGLACTIVETEXTUREPROC>(glfwGetProcAddress("glActiveTexture"));
    assert(mglActiveTexture && "glActiveTexture() import failed.");
    mglSampleCoverage = reinterpret_cast<PFNGLSAMPLECOVERAGEPROC>(glfwGetProcAddress("glSampleCoverage"));
    assert(mglSampleCoverage && "glSampleCoverage() import failed.");
    mglCompressedTexImage3D = reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE3DPROC>(glfwGetProcAddress("glCompressedTexImage3D"));
    assert(mglCompressedTexImage3D && "glCompressedTexImage3D() import failed.");
    mglCompressedTexImage2D = reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE2DPROC>(glfwGetProcAddress("glCompressedTexImage2D"));
    assert(mglCompressedTexImage2D && "glCompressedTexImage2D() import failed.");
    mglCompressedTexImage1D = reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE1DPROC>(glfwGetProcAddress("glCompressedTexImage1D"));
    assert(mglCompressedTexImage1D && "glCompressedTexImage1D() import failed.");
    mglCompressedTexSubImage3D = reinterpret_cast<PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC>(glfwGetProcAddress("glCompressedTexSubImage3D"));
    assert(mglCompressedTexSubImage3D && "glCompressedTexSubImage3D() import failed.");
    mglCompressedTexSubImage2D = reinterpret_cast<PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC>(glfwGetProcAddress("glCompressedTexSubImage2D"));
    assert(mglCompressedTexSubImage2D && "glCompressedTexSubImage2D() import failed.");
    mglCompressedTexSubImage1D = reinterpret_cast<PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC>(glfwGetProcAddress("glCompressedTexSubImage1D"));
    assert(mglCompressedTexSubImage1D && "glCompressedTexSubImage1D() import failed.");
    mglGetCompressedTexImage = reinterpret_cast<PFNGLGETCOMPRESSEDTEXIMAGEPROC>(glfwGetProcAddress("glGetCompressedTexImage"));
    assert(mglGetCompressedTexImage && "glGetCompressedTexImage() import failed.");

    // GL_VERSION_1_4
    mglBlendFuncSeparate = reinterpret_cast<PFNGLBLENDFUNCSEPARATEPROC>(glfwGetProcAddress("glBlendFuncSeparate"));
    assert(mglBlendFuncSeparate && "glBlendFuncSeparate() import failed.");
    mglMultiDrawArrays = reinterpret_cast<PFNGLMULTIDRAWARRAYSPROC>(glfwGetProcAddress("glMultiDrawArrays"));
    assert(mglMultiDrawArrays && "glMultiDrawArrays() import failed.");
    mglMultiDrawElements = reinterpret_cast<PFNGLMULTIDRAWELEMENTSPROC>(glfwGetProcAddress("glMultiDrawElements"));
    assert(mglMultiDrawElements && "glMultiDrawElements() import failed.");
    mglPointParameterf = reinterpret_cast<PFNGLPOINTPARAMETERFPROC>(glfwGetProcAddress("glPointParameterf"));
    assert(mglPointParameterf && "glPointParameterf() import failed.");
    mglPointParameterfv = reinterpret_cast<PFNGLPOINTPARAMETERFVPROC>(glfwGetProcAddress("glPointParameterfv"));
    assert(mglPointParameterfv && "glPointParameterfv() import failed.");
    mglPointParameteri = reinterpret_cast<PFNGLPOINTPARAMETERIPROC>(glfwGetProcAddress("glPointParameteri"));
    assert(mglPointParameteri && "glPointParameteri() import failed.");
    mglPointParameteriv = reinterpret_cast<PFNGLPOINTPARAMETERIVPROC>(glfwGetProcAddress("glPointParameteriv"));
    assert(mglPointParameteriv && "glPointParameteriv() import failed.");
    mglBlendColor = reinterpret_cast<PFNGLBLENDCOLORPROC>(glfwGetProcAddress("glBlendColor"));
    assert(mglBlendColor && "glBlendColor() import failed.");
    mglBlendEquation = reinterpret_cast<PFNGLBLENDEQUATIONPROC>(glfwGetProcAddress("glBlendEquation"));
    assert(mglBlendEquation && "glBlendEquation() import failed.");

    // GL_VERSION_1_5
    mglGenQueries = reinterpret_cast<PFNGLGENQUERIESPROC>(glfwGetProcAddress("glGenQueries"));
    assert(mglGenQueries && "glGenQueries() import failed.");
    mglDeleteQueries = reinterpret_cast<PFNGLDELETEQUERIESPROC>(glfwGetProcAddress("glDeleteQueries"));
    assert(mglDeleteQueries && "glDeleteQueries() import failed.");
    mglIsQuery = reinterpret_cast<PFNGLISQUERYPROC>(glfwGetProcAddress("glIsQuery"));
    assert(mglIsQuery && "glIsQuery() import failed.");
    mglBeginQuery = reinterpret_cast<PFNGLBEGINQUERYPROC>(glfwGetProcAddress("glBeginQuery"));
    assert(mglBeginQuery && "glBeginQuery() import failed.");
    mglEndQuery = reinterpret_cast<PFNGLENDQUERYPROC>(glfwGetProcAddress("glEndQuery"));
    assert(mglEndQuery && "glEndQuery() import failed.");
    mglGetQueryiv = reinterpret_cast<PFNGLGETQUERYIVPROC>(glfwGetProcAddress("glGetQueryiv"));
    assert(mglGetQueryiv && "glGetQueryiv() import failed.");
    mglGetQueryObjectiv = reinterpret_cast<PFNGLGETQUERYOBJECTIVPROC>(glfwGetProcAddress("glGetQueryObjectiv"));
    assert(mglGetQueryObjectiv && "glGetQueryObjectiv() import failed.");
    mglGetQueryObjectuiv = reinterpret_cast<PFNGLGETQUERYOBJECTUIVPROC>(glfwGetProcAddress("glGetQueryObjectuiv"));
    assert(mglGetQueryObjectuiv && "glGetQueryObjectuiv() import failed.");
    mglBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(glfwGetProcAddress("glBindBuffer"));
    assert(mglBindBuffer && "glBindBuffer() import failed.");
    mglDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(glfwGetProcAddress("glDeleteBuffers"));
    assert(mglDeleteBuffers && "glDeleteBuffers() import failed.");
    mglGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(glfwGetProcAddress("glGenBuffers"));
    assert(mglGenBuffers && "glGenBuffers() import failed.");
    mglIsBuffer = reinterpret_cast<PFNGLISBUFFERPROC>(glfwGetProcAddress("glIsBuffer"));
    assert(mglIsBuffer && "glIsBuffer() import failed.");
    mglBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(glfwGetProcAddress("glBufferData"));
    assert(mglBufferData && "glBufferData() import failed.");
    mglBufferSubData = reinterpret_cast<PFNGLBUFFERSUBDATAPROC>(glfwGetProcAddress("glBufferSubData"));
    assert(mglBufferSubData && "glBufferSubData() import failed.");
    mglGetBufferSubData = reinterpret_cast<PFNGLGETBUFFERSUBDATAPROC>(glfwGetProcAddress("glGetBufferSubData"));
    assert(mglGetBufferSubData && "glGetBufferSubData() import failed.");
    mglMapBuffer = reinterpret_cast<PFNGLMAPBUFFERPROC>(glfwGetProcAddress("glMapBuffer"));
    assert(mglMapBuffer && "glMapBuffer() import failed.");
    mglUnmapBuffer = reinterpret_cast<PFNGLUNMAPBUFFERPROC>(glfwGetProcAddress("glUnmapBuffer"));
    assert(mglUnmapBuffer && "glUnmapBuffer() import failed.");
    mglGetBufferParameteriv = reinterpret_cast<PFNGLGETBUFFERPARAMETERIVPROC>(glfwGetProcAddress("glGetBufferParameteriv"));
    assert(mglGetBufferParameteriv && "glGetBufferParameteriv() import failed.");
    mglGetBufferPointerv = reinterpret_cast<PFNGLGETBUFFERPOINTERVPROC>(glfwGetProcAddress("glGetBufferPointerv"));
    assert(mglGetBufferPointerv && "glGetBufferPointerv() import failed.");

    // GL_VERSION_2_0
    mglBlendEquationSeparate = reinterpret_cast<PFNGLBLENDEQUATIONSEPARATEPROC>(glfwGetProcAddress("glBlendEquationSeparate"));
    assert(mglBlendEquationSeparate && "glBlendEquationSeparate() import failed.");
    mglDrawBuffers = reinterpret_cast<PFNGLDRAWBUFFERSPROC>(glfwGetProcAddress("glDrawBuffers"));
    assert(mglDrawBuffers && "glDrawBuffers() import failed.");
    mglStencilOpSeparate = reinterpret_cast<PFNGLSTENCILOPSEPARATEPROC>(glfwGetProcAddress("glStencilOpSeparate"));
    assert(mglStencilOpSeparate && "glStencilOpSeparate() import failed.");
    mglStencilFuncSeparate = reinterpret_cast<PFNGLSTENCILFUNCSEPARATEPROC>(glfwGetProcAddress("glStencilFuncSeparate"));
    assert(mglStencilFuncSeparate && "glStencilFuncSeparate() import failed.");
    mglStencilMaskSeparate = reinterpret_cast<PFNGLSTENCILMASKSEPARATEPROC>(glfwGetProcAddress("glStencilMaskSeparate"));
    assert(mglStencilMaskSeparate && "glStencilMaskSeparate() import failed.");
    mglAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(glfwGetProcAddress("glAttachShader"));
    assert(mglAttachShader && "glAttachShader() import failed.");
    mglBindAttribLocation = reinterpret_cast<PFNGLBINDATTRIBLOCATIONPROC>(glfwGetProcAddress("glBindAttribLocation"));
    assert(mglBindAttribLocation && "glBindAttribLocation() import failed.");
    mglCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(glfwGetProcAddress("glCompileShader"));
    assert(mglCompileShader && "glCompileShader() import failed.");
    mglCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(glfwGetProcAddress("glCreateProgram"));
    assert(mglCreateProgram && "glCreateProgram() import failed.");
    mglCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(glfwGetProcAddress("glCreateShader"));
    assert(mglCreateShader && "glCreateShader() import failed.");
    mglDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(glfwGetProcAddress("glDeleteProgram"));
    assert(mglDeleteProgram && "glDeleteProgram() import failed.");
    mglDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>(glfwGetProcAddress("glDeleteShader"));
    assert(mglDeleteShader && "glDeleteShader() import failed.");
    mglDetachShader = reinterpret_cast<PFNGLDETACHSHADERPROC>(glfwGetProcAddress("glDetachShader"));
    assert(mglDetachShader && "glDetachShader() import failed.");
    mglDisableVertexAttribArray = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYPROC>(glfwGetProcAddress("glDisableVertexAttribArray"));
    assert(mglDisableVertexAttribArray && "glDisableVertexAttribArray() import failed.");
    mglEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(glfwGetProcAddress("glEnableVertexAttribArray"));
    assert(mglEnableVertexAttribArray && "glEnableVertexAttribArray() import failed.");
    mglGetActiveAttrib = reinterpret_cast<PFNGLGETACTIVEATTRIBPROC>(glfwGetProcAddress("glGetActiveAttrib"));
    assert(mglGetActiveAttrib && "glGetActiveAttrib() import failed.");
    mglGetActiveUniform = reinterpret_cast<PFNGLGETACTIVEUNIFORMPROC>(glfwGetProcAddress("glGetActiveUniform"));
    assert(mglGetActiveUniform && "glGetActiveUniform() import failed.");
    mglGetAttachedShaders = reinterpret_cast<PFNGLGETATTACHEDSHADERSPROC>(glfwGetProcAddress("glGetAttachedShaders"));
    assert(mglGetAttachedShaders && "glGetAttachedShaders() import failed.");
    mglGetAttribLocation = reinterpret_cast<PFNGLGETATTRIBLOCATIONPROC>(glfwGetProcAddress("glGetAttribLocation"));
    assert(mglGetAttribLocation && "glGetAttribLocation() import failed.");
    mglGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(glfwGetProcAddress("glGetProgramiv"));
    assert(mglGetProgramiv && "glGetProgramiv() import failed.");
    mglGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(glfwGetProcAddress("glGetProgramInfoLog"));
    assert(mglGetProgramInfoLog && "glGetProgramInfoLog() import failed.");
    mglGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>(glfwGetProcAddress("glGetShaderiv"));
    assert(mglGetShaderiv && "glGetShaderiv() import failed.");
    mglGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(glfwGetProcAddress("glGetShaderInfoLog"));
    assert(mglGetShaderInfoLog && "glGetShaderInfoLog() import failed.");
    mglGetShaderSource = reinterpret_cast<PFNGLGETSHADERSOURCEPROC>(glfwGetProcAddress("glGetShaderSource"));
    assert(mglGetShaderSource && "glGetShaderSource() import failed.");
    mglGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(glfwGetProcAddress("glGetUniformLocation"));
    assert(mglGetUniformLocation && "glGetUniformLocation() import failed.");
    mglGetUniformfv = reinterpret_cast<PFNGLGETUNIFORMFVPROC>(glfwGetProcAddress("glGetUniformfv"));
    assert(mglGetUniformfv && "glGetUniformfv() import failed.");
    mglGetUniformiv = reinterpret_cast<PFNGLGETUNIFORMIVPROC>(glfwGetProcAddress("glGetUniformiv"));
    assert(mglGetUniformiv && "glGetUniformiv() import failed.");
    mglGetVertexAttribdv = reinterpret_cast<PFNGLGETVERTEXATTRIBDVPROC>(glfwGetProcAddress("glGetVertexAttribdv"));
    assert(mglGetVertexAttribdv && "glGetVertexAttribdv() import failed.");
    mglGetVertexAttribfv = reinterpret_cast<PFNGLGETVERTEXATTRIBFVPROC>(glfwGetProcAddress("glGetVertexAttribfv"));
    assert(mglGetVertexAttribfv && "glGetVertexAttribfv() import failed.");
    mglGetVertexAttribiv = reinterpret_cast<PFNGLGETVERTEXATTRIBIVPROC>(glfwGetProcAddress("glGetVertexAttribiv"));
    assert(mglGetVertexAttribiv && "glGetVertexAttribiv() import failed.");
    mglGetVertexAttribPointerv = reinterpret_cast<PFNGLGETVERTEXATTRIBPOINTERVPROC>(glfwGetProcAddress("glGetVertexAttribPointerv"));
    assert(mglGetVertexAttribPointerv && "glGetVertexAttribPointerv() import failed.");
    mglIsProgram = reinterpret_cast<PFNGLISPROGRAMPROC>(glfwGetProcAddress("glIsProgram"));
    assert(mglIsProgram && "glIsProgram() import failed.");
    mglIsShader = reinterpret_cast<PFNGLISSHADERPROC>(glfwGetProcAddress("glIsShader"));
    assert(mglIsShader && "glIsShader() import failed.");
    mglLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(glfwGetProcAddress("glLinkProgram"));
    assert(mglLinkProgram && "glLinkProgram() import failed.");
    mglShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(glfwGetProcAddress("glShaderSource"));
    assert(mglShaderSource && "glShaderSource() import failed.");
    mglUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(glfwGetProcAddress("glUseProgram"));
    assert(mglUseProgram && "glUseProgram() import failed.");
    mglUniform1f = reinterpret_cast<PFNGLUNIFORM1FPROC>(glfwGetProcAddress("glUniform1f"));
    assert(mglUniform1f && "glUniform1f() import failed.");
    mglUniform2f = reinterpret_cast<PFNGLUNIFORM2FPROC>(glfwGetProcAddress("glUniform2f"));
    assert(mglUniform2f && "glUniform2f() import failed.");
    mglUniform3f = reinterpret_cast<PFNGLUNIFORM3FPROC>(glfwGetProcAddress("glUniform3f"));
    assert(mglUniform3f && "glUniform3f() import failed.");
    mglUniform4f = reinterpret_cast<PFNGLUNIFORM4FPROC>(glfwGetProcAddress("glUniform4f"));
    assert(mglUniform4f && "glUniform4f() import failed.");
    mglUniform1i = reinterpret_cast<PFNGLUNIFORM1IPROC>(glfwGetProcAddress("glUniform1i"));
    assert(mglUniform1i && "glUniform1i() import failed.");
    mglUniform2i = reinterpret_cast<PFNGLUNIFORM2IPROC>(glfwGetProcAddress("glUniform2i"));
    assert(mglUniform2i && "glUniform2i() import failed.");
    mglUniform3i = reinterpret_cast<PFNGLUNIFORM3IPROC>(glfwGetProcAddress("glUniform3i"));
    assert(mglUniform3i && "glUniform3i() import failed.");
    mglUniform4i = reinterpret_cast<PFNGLUNIFORM4IPROC>(glfwGetProcAddress("glUniform4i"));
    assert(mglUniform4i && "glUniform4i() import failed.");
    mglUniform1fv = reinterpret_cast<PFNGLUNIFORM1FVPROC>(glfwGetProcAddress("glUniform1fv"));
    assert(mglUniform1fv && "glUniform1fv() import failed.");
    mglUniform2fv = reinterpret_cast<PFNGLUNIFORM2FVPROC>(glfwGetProcAddress("glUniform2fv"));
    assert(mglUniform2fv && "glUniform2fv() import failed.");
    mglUniform3fv = reinterpret_cast<PFNGLUNIFORM3FVPROC>(glfwGetProcAddress("glUniform3fv"));
    assert(mglUniform3fv && "glUniform3fv() import failed.");
    mglUniform4fv = reinterpret_cast<PFNGLUNIFORM4FVPROC>(glfwGetProcAddress("glUniform4fv"));
    assert(mglUniform4fv && "glUniform4fv() import failed.");
    mglUniform1iv = reinterpret_cast<PFNGLUNIFORM1IVPROC>(glfwGetProcAddress("glUniform1iv"));
    assert(mglUniform1iv && "glUniform1iv() import failed.");
    mglUniform2iv = reinterpret_cast<PFNGLUNIFORM2IVPROC>(glfwGetProcAddress("glUniform2iv"));
    assert(mglUniform2iv && "glUniform2iv() import failed.");
    mglUniform3iv = reinterpret_cast<PFNGLUNIFORM3IVPROC>(glfwGetProcAddress("glUniform3iv"));
    assert(mglUniform3iv && "glUniform3iv() import failed.");
    mglUniform4iv = reinterpret_cast<PFNGLUNIFORM4IVPROC>(glfwGetProcAddress("glUniform4iv"));
    assert(mglUniform4iv && "glUniform4iv() import failed.");
    mglUniformMatrix2fv = reinterpret_cast<PFNGLUNIFORMMATRIX2FVPROC>(glfwGetProcAddress("glUniformMatrix2fv"));
    assert(mglUniformMatrix2fv && "glUniformMatrix2fv() import failed.");
    mglUniformMatrix3fv = reinterpret_cast<PFNGLUNIFORMMATRIX3FVPROC>(glfwGetProcAddress("glUniformMatrix3fv"));
    assert(mglUniformMatrix3fv && "glUniformMatrix3fv() import failed.");
    mglUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(glfwGetProcAddress("glUniformMatrix4fv"));
    assert(mglUniformMatrix4fv && "glUniformMatrix4fv() import failed.");
    mglValidateProgram = reinterpret_cast<PFNGLVALIDATEPROGRAMPROC>(glfwGetProcAddress("glValidateProgram"));
    assert(mglValidateProgram && "glValidateProgram() import failed.");
    mglVertexAttrib1d = reinterpret_cast<PFNGLVERTEXATTRIB1DPROC>(glfwGetProcAddress("glVertexAttrib1d"));
    assert(mglVertexAttrib1d && "glVertexAttrib1d() import failed.");
    mglVertexAttrib1dv = reinterpret_cast<PFNGLVERTEXATTRIB1DVPROC>(glfwGetProcAddress("glVertexAttrib1dv"));
    assert(mglVertexAttrib1dv && "glVertexAttrib1dv() import failed.");
    mglVertexAttrib1f = reinterpret_cast<PFNGLVERTEXATTRIB1FPROC>(glfwGetProcAddress("glVertexAttrib1f"));
    assert(mglVertexAttrib1f && "glVertexAttrib1f() import failed.");
    mglVertexAttrib1fv = reinterpret_cast<PFNGLVERTEXATTRIB1FVPROC>(glfwGetProcAddress("glVertexAttrib1fv"));
    assert(mglVertexAttrib1fv && "glVertexAttrib1fv() import failed.");
    mglVertexAttrib1s = reinterpret_cast<PFNGLVERTEXATTRIB1SPROC>(glfwGetProcAddress("glVertexAttrib1s"));
    assert(mglVertexAttrib1s && "glVertexAttrib1s() import failed.");
    mglVertexAttrib1sv = reinterpret_cast<PFNGLVERTEXATTRIB1SVPROC>(glfwGetProcAddress("glVertexAttrib1sv"));
    assert(mglVertexAttrib1sv && "glVertexAttrib1sv() import failed.");
    mglVertexAttrib2d = reinterpret_cast<PFNGLVERTEXATTRIB2DPROC>(glfwGetProcAddress("glVertexAttrib2d"));
    assert(mglVertexAttrib2d && "glVertexAttrib2d() import failed.");
    mglVertexAttrib2dv = reinterpret_cast<PFNGLVERTEXATTRIB2DVPROC>(glfwGetProcAddress("glVertexAttrib2dv"));
    assert(mglVertexAttrib2dv && "glVertexAttrib2dv() import failed.");
    mglVertexAttrib2f = reinterpret_cast<PFNGLVERTEXATTRIB2FPROC>(glfwGetProcAddress("glVertexAttrib2f"));
    assert(mglVertexAttrib2f && "glVertexAttrib2f() import failed.");
    mglVertexAttrib2fv = reinterpret_cast<PFNGLVERTEXATTRIB2FVPROC>(glfwGetProcAddress("glVertexAttrib2fv"));
    assert(mglVertexAttrib2fv && "glVertexAttrib2fv() import failed.");
    mglVertexAttrib2s = reinterpret_cast<PFNGLVERTEXATTRIB2SPROC>(glfwGetProcAddress("glVertexAttrib2s"));
    assert(mglVertexAttrib2s && "glVertexAttrib2s() import failed.");
    mglVertexAttrib2sv = reinterpret_cast<PFNGLVERTEXATTRIB2SVPROC>(glfwGetProcAddress("glVertexAttrib2sv"));
    assert(mglVertexAttrib2sv && "glVertexAttrib2sv() import failed.");
    mglVertexAttrib3d = reinterpret_cast<PFNGLVERTEXATTRIB3DPROC>(glfwGetProcAddress("glVertexAttrib3d"));
    assert(mglVertexAttrib3d && "glVertexAttrib3d() import failed.");
    mglVertexAttrib3dv = reinterpret_cast<PFNGLVERTEXATTRIB3DVPROC>(glfwGetProcAddress("glVertexAttrib3dv"));
    assert(mglVertexAttrib3dv && "glVertexAttrib3dv() import failed.");
    mglVertexAttrib3f = reinterpret_cast<PFNGLVERTEXATTRIB3FPROC>(glfwGetProcAddress("glVertexAttrib3f"));
    assert(mglVertexAttrib3f && "glVertexAttrib3f() import failed.");
    mglVertexAttrib3fv = reinterpret_cast<PFNGLVERTEXATTRIB3FVPROC>(glfwGetProcAddress("glVertexAttrib3fv"));
    assert(mglVertexAttrib3fv && "glVertexAttrib3fv() import failed.");
    mglVertexAttrib3s = reinterpret_cast<PFNGLVERTEXATTRIB3SPROC>(glfwGetProcAddress("glVertexAttrib3s"));
    assert(mglVertexAttrib3s && "glVertexAttrib3s() import failed.");
    mglVertexAttrib3sv = reinterpret_cast<PFNGLVERTEXATTRIB3SVPROC>(glfwGetProcAddress("glVertexAttrib3sv"));
    assert(mglVertexAttrib3sv && "glVertexAttrib3sv() import failed.");
    mglVertexAttrib4Nbv = reinterpret_cast<PFNGLVERTEXATTRIB4NBVPROC>(glfwGetProcAddress("glVertexAttrib4Nbv"));
    assert(mglVertexAttrib4Nbv && "glVertexAttrib4Nbv() import failed.");
    mglVertexAttrib4Niv = reinterpret_cast<PFNGLVERTEXATTRIB4NIVPROC>(glfwGetProcAddress("glVertexAttrib4Niv"));
    assert(mglVertexAttrib4Niv && "glVertexAttrib4Niv() import failed.");
    mglVertexAttrib4Nsv = reinterpret_cast<PFNGLVERTEXATTRIB4NSVPROC>(glfwGetProcAddress("glVertexAttrib4Nsv"));
    assert(mglVertexAttrib4Nsv && "glVertexAttrib4Nsv() import failed.");
    mglVertexAttrib4Nub = reinterpret_cast<PFNGLVERTEXATTRIB4NUBPROC>(glfwGetProcAddress("glVertexAttrib4Nub"));
    assert(mglVertexAttrib4Nub && "glVertexAttrib4Nub() import failed.");
    mglVertexAttrib4Nubv = reinterpret_cast<PFNGLVERTEXATTRIB4NUBVPROC>(glfwGetProcAddress("glVertexAttrib4Nubv"));
    assert(mglVertexAttrib4Nubv && "glVertexAttrib4Nubv() import failed.");
    mglVertexAttrib4Nuiv = reinterpret_cast<PFNGLVERTEXATTRIB4NUIVPROC>(glfwGetProcAddress("glVertexAttrib4Nuiv"));
    assert(mglVertexAttrib4Nuiv && "glVertexAttrib4Nuiv() import failed.");
    mglVertexAttrib4Nusv = reinterpret_cast<PFNGLVERTEXATTRIB4NUSVPROC>(glfwGetProcAddress("glVertexAttrib4Nusv"));
    assert(mglVertexAttrib4Nusv && "glVertexAttrib4Nusv() import failed.");
    mglVertexAttrib4bv = reinterpret_cast<PFNGLVERTEXATTRIB4BVPROC>(glfwGetProcAddress("glVertexAttrib4bv"));
    assert(mglVertexAttrib4bv && "glVertexAttrib4bv() import failed.");
    mglVertexAttrib4d = reinterpret_cast<PFNGLVERTEXATTRIB4DPROC>(glfwGetProcAddress("glVertexAttrib4d"));
    assert(mglVertexAttrib4d && "glVertexAttrib4d() import failed.");
    mglVertexAttrib4dv = reinterpret_cast<PFNGLVERTEXATTRIB4DVPROC>(glfwGetProcAddress("glVertexAttrib4dv"));
    assert(mglVertexAttrib4dv && "glVertexAttrib4dv() import failed.");
    mglVertexAttrib4f = reinterpret_cast<PFNGLVERTEXATTRIB4FPROC>(glfwGetProcAddress("glVertexAttrib4f"));
    assert(mglVertexAttrib4f && "glVertexAttrib4f() import failed.");
    mglVertexAttrib4fv = reinterpret_cast<PFNGLVERTEXATTRIB4FVPROC>(glfwGetProcAddress("glVertexAttrib4fv"));
    assert(mglVertexAttrib4fv && "glVertexAttrib4fv() import failed.");
    mglVertexAttrib4iv = reinterpret_cast<PFNGLVERTEXATTRIB4IVPROC>(glfwGetProcAddress("glVertexAttrib4iv"));
    assert(mglVertexAttrib4iv && "glVertexAttrib4iv() import failed.");
    mglVertexAttrib4s = reinterpret_cast<PFNGLVERTEXATTRIB4SPROC>(glfwGetProcAddress("glVertexAttrib4s"));
    assert(mglVertexAttrib4s && "glVertexAttrib4s() import failed.");
    mglVertexAttrib4sv = reinterpret_cast<PFNGLVERTEXATTRIB4SVPROC>(glfwGetProcAddress("glVertexAttrib4sv"));
    assert(mglVertexAttrib4sv && "glVertexAttrib4sv() import failed.");
    mglVertexAttrib4ubv = reinterpret_cast<PFNGLVERTEXATTRIB4UBVPROC>(glfwGetProcAddress("glVertexAttrib4ubv"));
    assert(mglVertexAttrib4ubv && "glVertexAttrib4ubv() import failed.");
    mglVertexAttrib4uiv = reinterpret_cast<PFNGLVERTEXATTRIB4UIVPROC>(glfwGetProcAddress("glVertexAttrib4uiv"));
    assert(mglVertexAttrib4uiv && "glVertexAttrib4uiv() import failed.");
    mglVertexAttrib4usv = reinterpret_cast<PFNGLVERTEXATTRIB4USVPROC>(glfwGetProcAddress("glVertexAttrib4usv"));
    assert(mglVertexAttrib4usv && "glVertexAttrib4usv() import failed.");
    mglVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(glfwGetProcAddress("glVertexAttribPointer"));
    assert(mglVertexAttribPointer && "glVertexAttribPointer() import failed.");

    // GL_VERSION_2_1
    mglUniformMatrix2x3fv = reinterpret_cast<PFNGLUNIFORMMATRIX2X3FVPROC>(glfwGetProcAddress("glUniformMatrix2x3fv"));
    assert(mglUniformMatrix2x3fv && "glUniformMatrix2x3fv() import failed.");
    mglUniformMatrix3x2fv = reinterpret_cast<PFNGLUNIFORMMATRIX3X2FVPROC>(glfwGetProcAddress("glUniformMatrix3x2fv"));
    assert(mglUniformMatrix3x2fv && "glUniformMatrix3x2fv() import failed.");
    mglUniformMatrix2x4fv = reinterpret_cast<PFNGLUNIFORMMATRIX2X4FVPROC>(glfwGetProcAddress("glUniformMatrix2x4fv"));
    assert(mglUniformMatrix2x4fv && "glUniformMatrix2x4fv() import failed.");
    mglUniformMatrix4x2fv = reinterpret_cast<PFNGLUNIFORMMATRIX4X2FVPROC>(glfwGetProcAddress("glUniformMatrix4x2fv"));
    assert(mglUniformMatrix4x2fv && "glUniformMatrix4x2fv() import failed.");
    mglUniformMatrix3x4fv = reinterpret_cast<PFNGLUNIFORMMATRIX3X4FVPROC>(glfwGetProcAddress("glUniformMatrix3x4fv"));
    assert(mglUniformMatrix3x4fv && "glUniformMatrix3x4fv() import failed.");
    mglUniformMatrix4x3fv = reinterpret_cast<PFNGLUNIFORMMATRIX4X3FVPROC>(glfwGetProcAddress("glUniformMatrix4x3fv"));
    assert(mglUniformMatrix4x3fv && "glUniformMatrix4x3fv() import failed.");

    // GL_VERSION_3_0
    mglColorMaski = reinterpret_cast<PFNGLCOLORMASKIPROC>(glfwGetProcAddress("glColorMaski"));
    assert(mglColorMaski && "glColorMaski() import failed.");
    mglGetBooleani_v = reinterpret_cast<PFNGLGETBOOLEANI_VPROC>(glfwGetProcAddress("glGetBooleani_v"));
    assert(mglGetBooleani_v && "glGetBooleani_v() import failed.");
    mglGetIntegeri_v = reinterpret_cast<PFNGLGETINTEGERI_VPROC>(glfwGetProcAddress("glGetIntegeri_v"));
    assert(mglGetIntegeri_v && "glGetIntegeri_v() import failed.");
    mglEnablei = reinterpret_cast<PFNGLENABLEIPROC>(glfwGetProcAddress("glEnablei"));
    assert(mglEnablei && "glEnablei() import failed.");
    mglDisablei = reinterpret_cast<PFNGLDISABLEIPROC>(glfwGetProcAddress("glDisablei"));
    assert(mglDisablei && "glDisablei() import failed.");
    mglIsEnabledi = reinterpret_cast<PFNGLISENABLEDIPROC>(glfwGetProcAddress("glIsEnabledi"));
    assert(mglIsEnabledi && "glIsEnabledi() import failed.");
    mglBeginTransformFeedback = reinterpret_cast<PFNGLBEGINTRANSFORMFEEDBACKPROC>(glfwGetProcAddress("glBeginTransformFeedback"));
    assert(mglBeginTransformFeedback && "glBeginTransformFeedback() import failed.");
    mglEndTransformFeedback = reinterpret_cast<PFNGLENDTRANSFORMFEEDBACKPROC>(glfwGetProcAddress("glEndTransformFeedback"));
    assert(mglEndTransformFeedback && "glEndTransformFeedback() import failed.");
    mglBindBufferRange = reinterpret_cast<PFNGLBINDBUFFERRANGEPROC>(glfwGetProcAddress("glBindBufferRange"));
    assert(mglBindBufferRange && "glBindBufferRange() import failed.");
    mglBindBufferBase = reinterpret_cast<PFNGLBINDBUFFERBASEPROC>(glfwGetProcAddress("glBindBufferBase"));
    assert(mglBindBufferBase && "glBindBufferBase() import failed.");
    mglTransformFeedbackVaryings = reinterpret_cast<PFNGLTRANSFORMFEEDBACKVARYINGSPROC>(glfwGetProcAddress("glTransformFeedbackVaryings"));
    assert(mglTransformFeedbackVaryings && "glTransformFeedbackVaryings() import failed.");
    mglGetTransformFeedbackVarying = reinterpret_cast<PFNGLGETTRANSFORMFEEDBACKVARYINGPROC>(glfwGetProcAddress("glGetTransformFeedbackVarying"));
    assert(mglGetTransformFeedbackVarying && "glGetTransformFeedbackVarying() import failed.");
    mglClampColor = reinterpret_cast<PFNGLCLAMPCOLORPROC>(glfwGetProcAddress("glClampColor"));
    assert(mglClampColor && "glClampColor() import failed.");
    mglBeginConditionalRender = reinterpret_cast<PFNGLBEGINCONDITIONALRENDERPROC>(glfwGetProcAddress("glBeginConditionalRender"));
    assert(mglBeginConditionalRender && "glBeginConditionalRender() import failed.");
    mglEndConditionalRender = reinterpret_cast<PFNGLENDCONDITIONALRENDERPROC>(glfwGetProcAddress("glEndConditionalRender"));
    assert(mglEndConditionalRender && "glEndConditionalRender() import failed.");
    mglVertexAttribIPointer = reinterpret_cast<PFNGLVERTEXATTRIBIPOINTERPROC>(glfwGetProcAddress("glVertexAttribIPointer"));
    assert(mglVertexAttribIPointer && "glVertexAttribIPointer() import failed.");
    mglGetVertexAttribIiv = reinterpret_cast<PFNGLGETVERTEXATTRIBIIVPROC>(glfwGetProcAddress("glGetVertexAttribIiv"));
    assert(mglGetVertexAttribIiv && "glGetVertexAttribIiv() import failed.");
    mglGetVertexAttribIuiv = reinterpret_cast<PFNGLGETVERTEXATTRIBIUIVPROC>(glfwGetProcAddress("glGetVertexAttribIuiv"));
    assert(mglGetVertexAttribIuiv && "glGetVertexAttribIuiv() import failed.");
    mglVertexAttribI1i = reinterpret_cast<PFNGLVERTEXATTRIBI1IPROC>(glfwGetProcAddress("glVertexAttribI1i"));
    assert(mglVertexAttribI1i && "glVertexAttribI1i() import failed.");
    mglVertexAttribI2i = reinterpret_cast<PFNGLVERTEXATTRIBI2IPROC>(glfwGetProcAddress("glVertexAttribI2i"));
    assert(mglVertexAttribI2i && "glVertexAttribI2i() import failed.");
    mglVertexAttribI3i = reinterpret_cast<PFNGLVERTEXATTRIBI3IPROC>(glfwGetProcAddress("glVertexAttribI3i"));
    assert(mglVertexAttribI3i && "glVertexAttribI3i() import failed.");
    mglVertexAttribI4i = reinterpret_cast<PFNGLVERTEXATTRIBI4IPROC>(glfwGetProcAddress("glVertexAttribI4i"));
    assert(mglVertexAttribI4i && "glVertexAttribI4i() import failed.");
    mglVertexAttribI1ui = reinterpret_cast<PFNGLVERTEXATTRIBI1UIPROC>(glfwGetProcAddress("glVertexAttribI1ui"));
    assert(mglVertexAttribI1ui && "glVertexAttribI1ui() import failed.");
    mglVertexAttribI2ui = reinterpret_cast<PFNGLVERTEXATTRIBI2UIPROC>(glfwGetProcAddress("glVertexAttribI2ui"));
    assert(mglVertexAttribI2ui && "glVertexAttribI2ui() import failed.");
    mglVertexAttribI3ui = reinterpret_cast<PFNGLVERTEXATTRIBI3UIPROC>(glfwGetProcAddress("glVertexAttribI3ui"));
    assert(mglVertexAttribI3ui && "glVertexAttribI3ui() import failed.");
    mglVertexAttribI4ui = reinterpret_cast<PFNGLVERTEXATTRIBI4UIPROC>(glfwGetProcAddress("glVertexAttribI4ui"));
    assert(mglVertexAttribI4ui && "glVertexAttribI4ui() import failed.");
    mglVertexAttribI1iv = reinterpret_cast<PFNGLVERTEXATTRIBI1IVPROC>(glfwGetProcAddress("glVertexAttribI1iv"));
    assert(mglVertexAttribI1iv && "glVertexAttribI1iv() import failed.");
    mglVertexAttribI2iv = reinterpret_cast<PFNGLVERTEXATTRIBI2IVPROC>(glfwGetProcAddress("glVertexAttribI2iv"));
    assert(mglVertexAttribI2iv && "glVertexAttribI2iv() import failed.");
    mglVertexAttribI3iv = reinterpret_cast<PFNGLVERTEXATTRIBI3IVPROC>(glfwGetProcAddress("glVertexAttribI3iv"));
    assert(mglVertexAttribI3iv && "glVertexAttribI3iv() import failed.");
    mglVertexAttribI4iv = reinterpret_cast<PFNGLVERTEXATTRIBI4IVPROC>(glfwGetProcAddress("glVertexAttribI4iv"));
    assert(mglVertexAttribI4iv && "glVertexAttribI4iv() import failed.");
    mglVertexAttribI1uiv = reinterpret_cast<PFNGLVERTEXATTRIBI1UIVPROC>(glfwGetProcAddress("glVertexAttribI1uiv"));
    assert(mglVertexAttribI1uiv && "glVertexAttribI1uiv() import failed.");
    mglVertexAttribI2uiv = reinterpret_cast<PFNGLVERTEXATTRIBI2UIVPROC>(glfwGetProcAddress("glVertexAttribI2uiv"));
    assert(mglVertexAttribI2uiv && "glVertexAttribI2uiv() import failed.");
    mglVertexAttribI3uiv = reinterpret_cast<PFNGLVERTEXATTRIBI3UIVPROC>(glfwGetProcAddress("glVertexAttribI3uiv"));
    assert(mglVertexAttribI3uiv && "glVertexAttribI3uiv() import failed.");
    mglVertexAttribI4uiv = reinterpret_cast<PFNGLVERTEXATTRIBI4UIVPROC>(glfwGetProcAddress("glVertexAttribI4uiv"));
    assert(mglVertexAttribI4uiv && "glVertexAttribI4uiv() import failed.");
    mglVertexAttribI4bv = reinterpret_cast<PFNGLVERTEXATTRIBI4BVPROC>(glfwGetProcAddress("glVertexAttribI4bv"));
    assert(mglVertexAttribI4bv && "glVertexAttribI4bv() import failed.");
    mglVertexAttribI4sv = reinterpret_cast<PFNGLVERTEXATTRIBI4SVPROC>(glfwGetProcAddress("glVertexAttribI4sv"));
    assert(mglVertexAttribI4sv && "glVertexAttribI4sv() import failed.");
    mglVertexAttribI4ubv = reinterpret_cast<PFNGLVERTEXATTRIBI4UBVPROC>(glfwGetProcAddress("glVertexAttribI4ubv"));
    assert(mglVertexAttribI4ubv && "glVertexAttribI4ubv() import failed.");
    mglVertexAttribI4usv = reinterpret_cast<PFNGLVERTEXATTRIBI4USVPROC>(glfwGetProcAddress("glVertexAttribI4usv"));
    assert(mglVertexAttribI4usv && "glVertexAttribI4usv() import failed.");
    mglGetUniformuiv = reinterpret_cast<PFNGLGETUNIFORMUIVPROC>(glfwGetProcAddress("glGetUniformuiv"));
    assert(mglGetUniformuiv && "glGetUniformuiv() import failed.");
    mglBindFragDataLocation = reinterpret_cast<PFNGLBINDFRAGDATALOCATIONPROC>(glfwGetProcAddress("glBindFragDataLocation"));
    assert(mglBindFragDataLocation && "glBindFragDataLocation() import failed.");
    mglGetFragDataLocation = reinterpret_cast<PFNGLGETFRAGDATALOCATIONPROC>(glfwGetProcAddress("glGetFragDataLocation"));
    assert(mglGetFragDataLocation && "glGetFragDataLocation() import failed.");
    mglUniform1ui = reinterpret_cast<PFNGLUNIFORM1UIPROC>(glfwGetProcAddress("glUniform1ui"));
    assert(mglUniform1ui && "glUniform1ui() import failed.");
    mglUniform2ui = reinterpret_cast<PFNGLUNIFORM2UIPROC>(glfwGetProcAddress("glUniform2ui"));
    assert(mglUniform2ui && "glUniform2ui() import failed.");
    mglUniform3ui = reinterpret_cast<PFNGLUNIFORM3UIPROC>(glfwGetProcAddress("glUniform3ui"));
    assert(mglUniform3ui && "glUniform3ui() import failed.");
    mglUniform4ui = reinterpret_cast<PFNGLUNIFORM4UIPROC>(glfwGetProcAddress("glUniform4ui"));
    assert(mglUniform4ui && "glUniform4ui() import failed.");
    mglUniform1uiv = reinterpret_cast<PFNGLUNIFORM1UIVPROC>(glfwGetProcAddress("glUniform1uiv"));
    assert(mglUniform1uiv && "glUniform1uiv() import failed.");
    mglUniform2uiv = reinterpret_cast<PFNGLUNIFORM2UIVPROC>(glfwGetProcAddress("glUniform2uiv"));
    assert(mglUniform2uiv && "glUniform2uiv() import failed.");
    mglUniform3uiv = reinterpret_cast<PFNGLUNIFORM3UIVPROC>(glfwGetProcAddress("glUniform3uiv"));
    assert(mglUniform3uiv && "glUniform3uiv() import failed.");
    mglUniform4uiv = reinterpret_cast<PFNGLUNIFORM4UIVPROC>(glfwGetProcAddress("glUniform4uiv"));
    assert(mglUniform4uiv && "glUniform4uiv() import failed.");
    mglTexParameterIiv = reinterpret_cast<PFNGLTEXPARAMETERIIVPROC>(glfwGetProcAddress("glTexParameterIiv"));
    assert(mglTexParameterIiv && "glTexParameterIiv() import failed.");
    mglTexParameterIuiv = reinterpret_cast<PFNGLTEXPARAMETERIUIVPROC>(glfwGetProcAddress("glTexParameterIuiv"));
    assert(mglTexParameterIuiv && "glTexParameterIuiv() import failed.");
    mglGetTexParameterIiv = reinterpret_cast<PFNGLGETTEXPARAMETERIIVPROC>(glfwGetProcAddress("glGetTexParameterIiv"));
    assert(mglGetTexParameterIiv && "glGetTexParameterIiv() import failed.");
    mglGetTexParameterIuiv = reinterpret_cast<PFNGLGETTEXPARAMETERIUIVPROC>(glfwGetProcAddress("glGetTexParameterIuiv"));
    assert(mglGetTexParameterIuiv && "glGetTexParameterIuiv() import failed.");
    mglClearBufferiv = reinterpret_cast<PFNGLCLEARBUFFERIVPROC>(glfwGetProcAddress("glClearBufferiv"));
    assert(mglClearBufferiv && "glClearBufferiv() import failed.");
    mglClearBufferuiv = reinterpret_cast<PFNGLCLEARBUFFERUIVPROC>(glfwGetProcAddress("glClearBufferuiv"));
    assert(mglClearBufferuiv && "glClearBufferuiv() import failed.");
    mglClearBufferfv = reinterpret_cast<PFNGLCLEARBUFFERFVPROC>(glfwGetProcAddress("glClearBufferfv"));
    assert(mglClearBufferfv && "glClearBufferfv() import failed.");
    mglClearBufferfi = reinterpret_cast<PFNGLCLEARBUFFERFIPROC>(glfwGetProcAddress("glClearBufferfi"));
    assert(mglClearBufferfi && "glClearBufferfi() import failed.");
    mglGetStringi = reinterpret_cast<PFNGLGETSTRINGIPROC>(glfwGetProcAddress("glGetStringi"));
    assert(mglGetStringi && "glGetStringi() import failed.");
    mglIsRenderbuffer = reinterpret_cast<PFNGLISRENDERBUFFERPROC>(glfwGetProcAddress("glIsRenderbuffer"));
    assert(mglIsRenderbuffer && "glIsRenderbuffer() import failed.");
    mglBindRenderbuffer = reinterpret_cast<PFNGLBINDRENDERBUFFERPROC>(glfwGetProcAddress("glBindRenderbuffer"));
    assert(mglBindRenderbuffer && "glBindRenderbuffer() import failed.");
    mglDeleteRenderbuffers = reinterpret_cast<PFNGLDELETERENDERBUFFERSPROC>(glfwGetProcAddress("glDeleteRenderbuffers"));
    assert(mglDeleteRenderbuffers && "glDeleteRenderbuffers() import failed.");
    mglGenRenderbuffers = reinterpret_cast<PFNGLGENRENDERBUFFERSPROC>(glfwGetProcAddress("glGenRenderbuffers"));
    assert(mglGenRenderbuffers && "glGenRenderbuffers() import failed.");
    mglRenderbufferStorage = reinterpret_cast<PFNGLRENDERBUFFERSTORAGEPROC>(glfwGetProcAddress("glRenderbufferStorage"));
    assert(mglRenderbufferStorage && "glRenderbufferStorage() import failed.");
    mglGetRenderbufferParameteriv = reinterpret_cast<PFNGLGETRENDERBUFFERPARAMETERIVPROC>(glfwGetProcAddress("glGetRenderbufferParameteriv"));
    assert(mglGetRenderbufferParameteriv && "glGetRenderbufferParameteriv() import failed.");
    mglIsFramebuffer = reinterpret_cast<PFNGLISFRAMEBUFFERPROC>(glfwGetProcAddress("glIsFramebuffer"));
    assert(mglIsFramebuffer && "glIsFramebuffer() import failed.");
    mglBindFramebuffer = reinterpret_cast<PFNGLBINDFRAMEBUFFERPROC>(glfwGetProcAddress("glBindFramebuffer"));
    assert(mglBindFramebuffer && "glBindFramebuffer() import failed.");
    mglDeleteFramebuffers = reinterpret_cast<PFNGLDELETEFRAMEBUFFERSPROC>(glfwGetProcAddress("glDeleteFramebuffers"));
    assert(mglDeleteFramebuffers && "glDeleteFramebuffers() import failed.");
    mglGenFramebuffers = reinterpret_cast<PFNGLGENFRAMEBUFFERSPROC>(glfwGetProcAddress("glGenFramebuffers"));
    assert(mglGenFramebuffers && "glGenFramebuffers() import failed.");
    mglCheckFramebufferStatus = reinterpret_cast<PFNGLCHECKFRAMEBUFFERSTATUSPROC>(glfwGetProcAddress("glCheckFramebufferStatus"));
    assert(mglCheckFramebufferStatus && "glCheckFramebufferStatus() import failed.");
    mglFramebufferTexture1D = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE1DPROC>(glfwGetProcAddress("glFramebufferTexture1D"));
    assert(mglFramebufferTexture1D && "glFramebufferTexture1D() import failed.");
    mglFramebufferTexture2D = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DPROC>(glfwGetProcAddress("glFramebufferTexture2D"));
    assert(mglFramebufferTexture2D && "glFramebufferTexture2D() import failed.");
    mglFramebufferTexture3D = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE3DPROC>(glfwGetProcAddress("glFramebufferTexture3D"));
    assert(mglFramebufferTexture3D && "glFramebufferTexture3D() import failed.");
    mglFramebufferRenderbuffer = reinterpret_cast<PFNGLFRAMEBUFFERRENDERBUFFERPROC>(glfwGetProcAddress("glFramebufferRenderbuffer"));
    assert(mglFramebufferRenderbuffer && "glFramebufferRenderbuffer() import failed.");
    mglGetFramebufferAttachmentParameteriv = reinterpret_cast<PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC>(glfwGetProcAddress("glGetFramebufferAttachmentParameteriv"));
    assert(mglGetFramebufferAttachmentParameteriv && "glGetFramebufferAttachmentParameteriv() import failed.");
    mglGenerateMipmap = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(glfwGetProcAddress("glGenerateMipmap"));
    assert(mglGenerateMipmap && "glGenerateMipmap() import failed.");
    mglBlitFramebuffer = reinterpret_cast<PFNGLBLITFRAMEBUFFERPROC>(glfwGetProcAddress("glBlitFramebuffer"));
    assert(mglBlitFramebuffer && "glBlitFramebuffer() import failed.");
    mglRenderbufferStorageMultisample = reinterpret_cast<PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC>(glfwGetProcAddress("glRenderbufferStorageMultisample"));
    assert(mglRenderbufferStorageMultisample && "glRenderbufferStorageMultisample() import failed.");
    mglFramebufferTextureLayer = reinterpret_cast<PFNGLFRAMEBUFFERTEXTURELAYERPROC>(glfwGetProcAddress("glFramebufferTextureLayer"));
    assert(mglFramebufferTextureLayer && "glFramebufferTextureLayer() import failed.");
    mglMapBufferRange = reinterpret_cast<PFNGLMAPBUFFERRANGEPROC>(glfwGetProcAddress("glMapBufferRange"));
    assert(mglMapBufferRange && "glMapBufferRange() import failed.");
    mglFlushMappedBufferRange = reinterpret_cast<PFNGLFLUSHMAPPEDBUFFERRANGEPROC>(glfwGetProcAddress("glFlushMappedBufferRange"));
    assert(mglFlushMappedBufferRange && "glFlushMappedBufferRange() import failed.");
    mglBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(glfwGetProcAddress("glBindVertexArray"));
    assert(mglBindVertexArray && "glBindVertexArray() import failed.");
    mglDeleteVertexArrays = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(glfwGetProcAddress("glDeleteVertexArrays"));
    assert(mglDeleteVertexArrays && "glDeleteVertexArrays() import failed.");
    mglGenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(glfwGetProcAddress("glGenVertexArrays"));
    assert(mglGenVertexArrays && "glGenVertexArrays() import failed.");
    mglIsVertexArray = reinterpret_cast<PFNGLISVERTEXARRAYPROC>(glfwGetProcAddress("glIsVertexArray"));
    assert(mglIsVertexArray && "glIsVertexArray() import failed.");

    // GL_VERSION_3_1
    mglDrawArraysInstanced = reinterpret_cast<PFNGLDRAWARRAYSINSTANCEDPROC>(glfwGetProcAddress("glDrawArraysInstanced"));
    assert(mglDrawArraysInstanced && "glDrawArraysInstanced() import failed.");
    mglDrawElementsInstanced = reinterpret_cast<PFNGLDRAWELEMENTSINSTANCEDPROC>(glfwGetProcAddress("glDrawElementsInstanced"));
    assert(mglDrawElementsInstanced && "glDrawElementsInstanced() import failed.");
    mglTexBuffer = reinterpret_cast<PFNGLTEXBUFFERPROC>(glfwGetProcAddress("glTexBuffer"));
    assert(mglTexBuffer && "glTexBuffer() import failed.");
    mglPrimitiveRestartIndex = reinterpret_cast<PFNGLPRIMITIVERESTARTINDEXPROC>(glfwGetProcAddress("glPrimitiveRestartIndex"));
    assert(mglPrimitiveRestartIndex && "glPrimitiveRestartIndex() import failed.");
    mglCopyBufferSubData = reinterpret_cast<PFNGLCOPYBUFFERSUBDATAPROC>(glfwGetProcAddress("glCopyBufferSubData"));
    assert(mglCopyBufferSubData && "glCopyBufferSubData() import failed.");
    mglGetUniformIndices = reinterpret_cast<PFNGLGETUNIFORMINDICESPROC>(glfwGetProcAddress("glGetUniformIndices"));
    assert(mglGetUniformIndices && "glGetUniformIndices() import failed.");
    mglGetActiveUniformsiv = reinterpret_cast<PFNGLGETACTIVEUNIFORMSIVPROC>(glfwGetProcAddress("glGetActiveUniformsiv"));
    assert(mglGetActiveUniformsiv && "glGetActiveUniformsiv() import failed.");
    mglGetActiveUniformName = reinterpret_cast<PFNGLGETACTIVEUNIFORMNAMEPROC>(glfwGetProcAddress("glGetActiveUniformName"));
    assert(mglGetActiveUniformName && "glGetActiveUniformName() import failed.");
    mglGetUniformBlockIndex = reinterpret_cast<PFNGLGETUNIFORMBLOCKINDEXPROC>(glfwGetProcAddress("glGetUniformBlockIndex"));
    assert(mglGetUniformBlockIndex && "glGetUniformBlockIndex() import failed.");
    mglGetActiveUniformBlockiv = reinterpret_cast<PFNGLGETACTIVEUNIFORMBLOCKIVPROC>(glfwGetProcAddress("glGetActiveUniformBlockiv"));
    assert(mglGetActiveUniformBlockiv && "glGetActiveUniformBlockiv() import failed.");
    mglGetActiveUniformBlockName = reinterpret_cast<PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC>(glfwGetProcAddress("glGetActiveUniformBlockName"));
    assert(mglGetActiveUniformBlockName && "glGetActiveUniformBlockName() import failed.");
    mglUniformBlockBinding = reinterpret_cast<PFNGLUNIFORMBLOCKBINDINGPROC>(glfwGetProcAddress("glUniformBlockBinding"));
    assert(mglUniformBlockBinding && "glUniformBlockBinding() import failed.");

    // GL_VERSION_3_2
    mglDrawElementsBaseVertex = reinterpret_cast<PFNGLDRAWELEMENTSBASEVERTEXPROC>(glfwGetProcAddress("glDrawElementsBaseVertex"));
    assert(mglDrawElementsBaseVertex && "glDrawElementsBaseVertex() import failed.");
    mglDrawRangeElementsBaseVertex = reinterpret_cast<PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC>(glfwGetProcAddress("glDrawRangeElementsBaseVertex"));
    assert(mglDrawRangeElementsBaseVertex && "glDrawRangeElementsBaseVertex() import failed.");
    mglDrawElementsInstancedBaseVertex = reinterpret_cast<PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC>(glfwGetProcAddress("glDrawElementsInstancedBaseVertex"));
    assert(mglDrawElementsInstancedBaseVertex && "glDrawElementsInstancedBaseVertex() import failed.");
    mglMultiDrawElementsBaseVertex = reinterpret_cast<PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC>(glfwGetProcAddress("glMultiDrawElementsBaseVertex"));
    assert(mglMultiDrawElementsBaseVertex && "glMultiDrawElementsBaseVertex() import failed.");
    mglProvokingVertex = reinterpret_cast<PFNGLPROVOKINGVERTEXPROC>(glfwGetProcAddress("glProvokingVertex"));
    assert(mglProvokingVertex && "glProvokingVertex() import failed.");
    mglFenceSync = reinterpret_cast<PFNGLFENCESYNCPROC>(glfwGetProcAddress("glFenceSync"));
    assert(mglFenceSync && "glFenceSync() import failed.");
    mglIsSync = reinterpret_cast<PFNGLISSYNCPROC>(glfwGetProcAddress("glIsSync"));
    assert(mglIsSync && "glIsSync() import failed.");
    mglDeleteSync = reinterpret_cast<PFNGLDELETESYNCPROC>(glfwGetProcAddress("glDeleteSync"));
    assert(mglDeleteSync && "glDeleteSync() import failed.");
    mglClientWaitSync = reinterpret_cast<PFNGLCLIENTWAITSYNCPROC>(glfwGetProcAddress("glClientWaitSync"));
    assert(mglClientWaitSync && "glClientWaitSync() import failed.");
    mglWaitSync = reinterpret_cast<PFNGLWAITSYNCPROC>(glfwGetProcAddress("glWaitSync"));
    assert(mglWaitSync && "glWaitSync() import failed.");
    mglGetInteger64v = reinterpret_cast<PFNGLGETINTEGER64VPROC>(glfwGetProcAddress("glGetInteger64v"));
    assert(mglGetInteger64v && "glGetInteger64v() import failed.");
    mglGetSynciv = reinterpret_cast<PFNGLGETSYNCIVPROC>(glfwGetProcAddress("glGetSynciv"));
    assert(mglGetSynciv && "glGetSynciv() import failed.");
    mglGetInteger64i_v = reinterpret_cast<PFNGLGETINTEGER64I_VPROC>(glfwGetProcAddress("glGetInteger64i_v"));
    assert(mglGetInteger64i_v && "glGetInteger64i_v() import failed.");
    mglGetBufferParameteri64v = reinterpret_cast<PFNGLGETBUFFERPARAMETERI64VPROC>(glfwGetProcAddress("glGetBufferParameteri64v"));
    assert(mglGetBufferParameteri64v && "glGetBufferParameteri64v() import failed.");
    mglFramebufferTexture = reinterpret_cast<PFNGLFRAMEBUFFERTEXTUREPROC>(glfwGetProcAddress("glFramebufferTexture"));
    assert(mglFramebufferTexture && "glFramebufferTexture() import failed.");
    mglTexImage2DMultisample = reinterpret_cast<PFNGLTEXIMAGE2DMULTISAMPLEPROC>(glfwGetProcAddress("glTexImage2DMultisample"));
    assert(mglTexImage2DMultisample && "glTexImage2DMultisample() import failed.");
    mglTexImage3DMultisample = reinterpret_cast<PFNGLTEXIMAGE3DMULTISAMPLEPROC>(glfwGetProcAddress("glTexImage3DMultisample"));
    assert(mglTexImage3DMultisample && "glTexImage3DMultisample() import failed.");
    mglGetMultisamplefv = reinterpret_cast<PFNGLGETMULTISAMPLEFVPROC>(glfwGetProcAddress("glGetMultisamplefv"));
    assert(mglGetMultisamplefv && "glGetMultisamplefv() import failed.");
    mglSampleMaski = reinterpret_cast<PFNGLSAMPLEMASKIPROC>(glfwGetProcAddress("glSampleMaski"));
    assert(mglSampleMaski && "glSampleMaski() import failed.");

    // GL_VERSION_3_3
    mglBindFragDataLocationIndexed = reinterpret_cast<PFNGLBINDFRAGDATALOCATIONINDEXEDPROC>(glfwGetProcAddress("glBindFragDataLocationIndexed"));
    assert(mglBindFragDataLocationIndexed && "glBindFragDataLocationIndexed() import failed.");
    mglGetFragDataIndex = reinterpret_cast<PFNGLGETFRAGDATAINDEXPROC>(glfwGetProcAddress("glGetFragDataIndex"));
    assert(mglGetFragDataIndex && "glGetFragDataIndex() import failed.");
    mglGenSamplers = reinterpret_cast<PFNGLGENSAMPLERSPROC>(glfwGetProcAddress("glGenSamplers"));
    assert(mglGenSamplers && "glGenSamplers() import failed.");
    mglDeleteSamplers = reinterpret_cast<PFNGLDELETESAMPLERSPROC>(glfwGetProcAddress("glDeleteSamplers"));
    assert(mglDeleteSamplers && "glDeleteSamplers() import failed.");
    mglIsSampler = reinterpret_cast<PFNGLISSAMPLERPROC>(glfwGetProcAddress("glIsSampler"));
    assert(mglIsSampler && "glIsSampler() import failed.");
    mglBindSampler = reinterpret_cast<PFNGLBINDSAMPLERPROC>(glfwGetProcAddress("glBindSampler"));
    assert(mglBindSampler && "glBindSampler() import failed.");
    mglSamplerParameteri = reinterpret_cast<PFNGLSAMPLERPARAMETERIPROC>(glfwGetProcAddress("glSamplerParameteri"));
    assert(mglSamplerParameteri && "glSamplerParameteri() import failed.");
    mglSamplerParameteriv = reinterpret_cast<PFNGLSAMPLERPARAMETERIVPROC>(glfwGetProcAddress("glSamplerParameteriv"));
    assert(mglSamplerParameteriv && "glSamplerParameteriv() import failed.");
    mglSamplerParameterf = reinterpret_cast<PFNGLSAMPLERPARAMETERFPROC>(glfwGetProcAddress("glSamplerParameterf"));
    assert(mglSamplerParameterf && "glSamplerParameterf() import failed.");
    mglSamplerParameterfv = reinterpret_cast<PFNGLSAMPLERPARAMETERFVPROC>(glfwGetProcAddress("glSamplerParameterfv"));
    assert(mglSamplerParameterfv && "glSamplerParameterfv() import failed.");
    mglSamplerParameterIiv = reinterpret_cast<PFNGLSAMPLERPARAMETERIIVPROC>(glfwGetProcAddress("glSamplerParameterIiv"));
    assert(mglSamplerParameterIiv && "glSamplerParameterIiv() import failed.");
    mglSamplerParameterIuiv = reinterpret_cast<PFNGLSAMPLERPARAMETERIUIVPROC>(glfwGetProcAddress("glSamplerParameterIuiv"));
    assert(mglSamplerParameterIuiv && "glSamplerParameterIuiv() import failed.");
    mglGetSamplerParameteriv = reinterpret_cast<PFNGLGETSAMPLERPARAMETERIVPROC>(glfwGetProcAddress("glGetSamplerParameteriv"));
    assert(mglGetSamplerParameteriv && "glGetSamplerParameteriv() import failed.");
    mglGetSamplerParameterIiv = reinterpret_cast<PFNGLGETSAMPLERPARAMETERIIVPROC>(glfwGetProcAddress("glGetSamplerParameterIiv"));
    assert(mglGetSamplerParameterIiv && "glGetSamplerParameterIiv() import failed.");
    mglGetSamplerParameterfv = reinterpret_cast<PFNGLGETSAMPLERPARAMETERFVPROC>(glfwGetProcAddress("glGetSamplerParameterfv"));
    assert(mglGetSamplerParameterfv && "glGetSamplerParameterfv() import failed.");
    mglGetSamplerParameterIuiv = reinterpret_cast<PFNGLGETSAMPLERPARAMETERIUIVPROC>(glfwGetProcAddress("glGetSamplerParameterIuiv"));
    assert(mglGetSamplerParameterIuiv && "glGetSamplerParameterIuiv() import failed.");
    mglQueryCounter = reinterpret_cast<PFNGLQUERYCOUNTERPROC>(glfwGetProcAddress("glQueryCounter"));
    assert(mglQueryCounter && "glQueryCounter() import failed.");
    mglGetQueryObjecti64v = reinterpret_cast<PFNGLGETQUERYOBJECTI64VPROC>(glfwGetProcAddress("glGetQueryObjecti64v"));
    assert(mglGetQueryObjecti64v && "glGetQueryObjecti64v() import failed.");
    mglGetQueryObjectui64v = reinterpret_cast<PFNGLGETQUERYOBJECTUI64VPROC>(glfwGetProcAddress("glGetQueryObjectui64v"));
    assert(mglGetQueryObjectui64v && "glGetQueryObjectui64v() import failed.");
    mglVertexAttribDivisor = reinterpret_cast<PFNGLVERTEXATTRIBDIVISORPROC>(glfwGetProcAddress("glVertexAttribDivisor"));
    assert(mglVertexAttribDivisor && "glVertexAttribDivisor() import failed.");
    mglVertexAttribP1ui = reinterpret_cast<PFNGLVERTEXATTRIBP1UIPROC>(glfwGetProcAddress("glVertexAttribP1ui"));
    assert(mglVertexAttribP1ui && "glVertexAttribP1ui() import failed.");
    mglVertexAttribP1uiv = reinterpret_cast<PFNGLVERTEXATTRIBP1UIVPROC>(glfwGetProcAddress("glVertexAttribP1uiv"));
    assert(mglVertexAttribP1uiv && "glVertexAttribP1uiv() import failed.");
    mglVertexAttribP2ui = reinterpret_cast<PFNGLVERTEXATTRIBP2UIPROC>(glfwGetProcAddress("glVertexAttribP2ui"));
    assert(mglVertexAttribP2ui && "glVertexAttribP2ui() import failed.");
    mglVertexAttribP2uiv = reinterpret_cast<PFNGLVERTEXATTRIBP2UIVPROC>(glfwGetProcAddress("glVertexAttribP2uiv"));
    assert(mglVertexAttribP2uiv && "glVertexAttribP2uiv() import failed.");
    mglVertexAttribP3ui = reinterpret_cast<PFNGLVERTEXATTRIBP3UIPROC>(glfwGetProcAddress("glVertexAttribP3ui"));
    assert(mglVertexAttribP3ui && "glVertexAttribP3ui() import failed.");
    mglVertexAttribP3uiv = reinterpret_cast<PFNGLVERTEXATTRIBP3UIVPROC>(glfwGetProcAddress("glVertexAttribP3uiv"));
    assert(mglVertexAttribP3uiv && "glVertexAttribP3uiv() import failed.");
    mglVertexAttribP4ui = reinterpret_cast<PFNGLVERTEXATTRIBP4UIPROC>(glfwGetProcAddress("glVertexAttribP4ui"));
    assert(mglVertexAttribP4ui && "glVertexAttribP4ui() import failed.");
    mglVertexAttribP4uiv = reinterpret_cast<PFNGLVERTEXATTRIBP4UIVPROC>(glfwGetProcAddress("glVertexAttribP4uiv"));
    assert(mglVertexAttribP4uiv && "glVertexAttribP4uiv() import failed.");

    // GL_ARB_debug_output
    if (glfwExtensionSupported("GL_ARB_debug_output"))
    {
        mglDebugMessageControlARB = reinterpret_cast<PFNGLDEBUGMESSAGECONTROLARBPROC>(glfwGetProcAddress("glDebugMessageControlARB"));
        assert(mglDebugMessageControlARB && "glDebugMessageControlARB() import failed.");
        mglDebugMessageInsertARB = reinterpret_cast<PFNGLDEBUGMESSAGEINSERTARBPROC>(glfwGetProcAddress("glDebugMessageInsertARB"));
        assert(mglDebugMessageInsertARB && "glDebugMessageInsertARB() import failed.");
        mglDebugMessageCallbackARB = reinterpret_cast<PFNGLDEBUGMESSAGECALLBACKARBPROC>(glfwGetProcAddress("glDebugMessageCallbackARB"));
        assert(mglDebugMessageCallbackARB && "glDebugMessageCallbackARB() import failed.");
        mglGetDebugMessageLogARB = reinterpret_cast<PFNGLGETDEBUGMESSAGELOGARBPROC>(glfwGetProcAddress("glGetDebugMessageLogARB"));
        assert(mglGetDebugMessageLogARB && "glGetDebugMessageLogARB() import failed.");
    }
}

} // anonymous namespace

namespace gl
{

void import()
{
    static std::once_flag m_flag;
    std::call_once(m_flag, import_impl);
}

// GL wrapper function definitions

// GL_VERSION_1_0

void CullFace(GLenum mode)
{
    assert(mglCullFace);
    mglCullFace(mode);
}

void FrontFace(GLenum mode)
{
    assert(mglFrontFace);
    mglFrontFace(mode);
}

void Hint(GLenum target, GLenum mode)
{
    assert(mglHint);
    mglHint(target, mode);
}

void LineWidth(GLfloat width)
{
    assert(mglLineWidth);
    mglLineWidth(width);
}

void PointSize(GLfloat size)
{
    assert(mglPointSize);
    mglPointSize(size);
}

void PolygonMode(GLenum face, GLenum mode)
{
    assert(mglPolygonMode);
    mglPolygonMode(face, mode);
}

void Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    assert(mglScissor);
    mglScissor(x, y, width, height);
}

void TexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    assert(mglTexParameterf);
    mglTexParameterf(target, pname, param);
}

void TexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
    assert(mglTexParameterfv);
    mglTexParameterfv(target, pname, params);
}

void TexParameteri(GLenum target, GLenum pname, GLint param)
{
    assert(mglTexParameteri);
    mglTexParameteri(target, pname, param);
}

void TexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
    assert(mglTexParameteriv);
    mglTexParameteriv(target, pname, params);
}

void TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels)
{
    assert(mglTexImage1D);
    mglTexImage1D(target, level, internalformat, width, border, format, type, pixels);
}

void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
{
    assert(mglTexImage2D);
    mglTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void DrawBuffer(GLenum buf)
{
    assert(mglDrawBuffer);
    mglDrawBuffer(buf);
}

void Clear(GLbitfield mask)
{
    assert(mglClear);
    mglClear(mask);
}

void ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    assert(mglClearColor);
    mglClearColor(red, green, blue, alpha);
}

void ClearStencil(GLint s)
{
    assert(mglClearStencil);
    mglClearStencil(s);
}

void ClearDepth(GLdouble depth)
{
    assert(mglClearDepth);
    mglClearDepth(depth);
}

void StencilMask(GLuint mask)
{
    assert(mglStencilMask);
    mglStencilMask(mask);
}

void ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    assert(mglColorMask);
    mglColorMask(red, green, blue, alpha);
}

void DepthMask(GLboolean flag)
{
    assert(mglDepthMask);
    mglDepthMask(flag);
}

void Disable(GLenum cap)
{
    assert(mglDisable);
    mglDisable(cap);
}

void Enable(GLenum cap)
{
    assert(mglEnable);
    mglEnable(cap);
}

void Finish()
{
    assert(mglFinish);
    mglFinish();
}

void Flush()
{
    assert(mglFlush);
    mglFlush();
}

void BlendFunc(GLenum sfactor, GLenum dfactor)
{
    assert(mglBlendFunc);
    mglBlendFunc(sfactor, dfactor);
}

void LogicOp(GLenum opcode)
{
    assert(mglLogicOp);
    mglLogicOp(opcode);
}

void StencilFunc(GLenum func, GLint ref, GLuint mask)
{
    assert(mglStencilFunc);
    mglStencilFunc(func, ref, mask);
}

void StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    assert(mglStencilOp);
    mglStencilOp(fail, zfail, zpass);
}

void DepthFunc(GLenum func)
{
    assert(mglDepthFunc);
    mglDepthFunc(func);
}

void PixelStoref(GLenum pname, GLfloat param)
{
    assert(mglPixelStoref);
    mglPixelStoref(pname, param);
}

void PixelStorei(GLenum pname, GLint param)
{
    assert(mglPixelStorei);
    mglPixelStorei(pname, param);
}

void ReadBuffer(GLenum src)
{
    assert(mglReadBuffer);
    mglReadBuffer(src);
}

void ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels)
{
    assert(mglReadPixels);
    mglReadPixels(x, y, width, height, format, type, pixels);
}

void GetBooleanv(GLenum pname, GLboolean* data)
{
    assert(mglGetBooleanv);
    mglGetBooleanv(pname, data);
}

void GetDoublev(GLenum pname, GLdouble* data)
{
    assert(mglGetDoublev);
    mglGetDoublev(pname, data);
}

GLenum GetError()
{
    assert(mglGetError);
    return mglGetError();
}

void GetFloatv(GLenum pname, GLfloat* data)
{
    assert(mglGetFloatv);
    mglGetFloatv(pname, data);
}

void GetIntegerv(GLenum pname, GLint* data)
{
    assert(mglGetIntegerv);
    mglGetIntegerv(pname, data);
}

const GLubyte* GetString(GLenum name)
{
    assert(mglGetString);
    return mglGetString(name);
}

void GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void* pixels)
{
    assert(mglGetTexImage);
    mglGetTexImage(target, level, format, type, pixels);
}

void GetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
    assert(mglGetTexParameterfv);
    mglGetTexParameterfv(target, pname, params);
}

void GetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
    assert(mglGetTexParameteriv);
    mglGetTexParameteriv(target, pname, params);
}

void GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat* params)
{
    assert(mglGetTexLevelParameterfv);
    mglGetTexLevelParameterfv(target, level, pname, params);
}

void GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint* params)
{
    assert(mglGetTexLevelParameteriv);
    mglGetTexLevelParameteriv(target, level, pname, params);
}

GLboolean IsEnabled(GLenum cap)
{
    assert(mglIsEnabled);
    return mglIsEnabled(cap);
}

void DepthRange(GLdouble near, GLdouble far)
{
    assert(mglDepthRange);
    mglDepthRange(near, far);
}

void Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    assert(mglViewport);
    mglViewport(x, y, width, height);
}

// GL_VERSION_1_1

void DrawArrays(GLenum mode, GLint first, GLsizei count)
{
    assert(mglDrawArrays);
    mglDrawArrays(mode, first, count);
}

void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
    assert(mglDrawElements);
    mglDrawElements(mode, count, type, indices);
}

void GetPointerv(GLenum pname, void** params)
{
    assert(mglGetPointerv);
    mglGetPointerv(pname, params);
}

void PolygonOffset(GLfloat factor, GLfloat units)
{
    assert(mglPolygonOffset);
    mglPolygonOffset(factor, units);
}

void CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
{
    assert(mglCopyTexImage1D);
    mglCopyTexImage1D(target, level, internalformat, x, y, width, border);
}

void CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    assert(mglCopyTexImage2D);
    mglCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
    assert(mglCopyTexSubImage1D);
    mglCopyTexSubImage1D(target, level, xoffset, x, y, width);
}

void CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    assert(mglCopyTexSubImage2D);
    mglCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels)
{
    assert(mglTexSubImage1D);
    mglTexSubImage1D(target, level, xoffset, width, format, type, pixels);
}

void TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
    assert(mglTexSubImage2D);
    mglTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void BindTexture(GLenum target, GLuint texture)
{
    assert(mglBindTexture);
    mglBindTexture(target, texture);
}

void DeleteTextures(GLsizei n, const GLuint* textures)
{
    assert(mglDeleteTextures);
    mglDeleteTextures(n, textures);
}

void GenTextures(GLsizei n, GLuint* textures)
{
    assert(mglGenTextures);
    mglGenTextures(n, textures);
}

GLboolean IsTexture(GLuint texture)
{
    assert(mglIsTexture);
    return mglIsTexture(texture);
}

// GL_VERSION_1_2

void DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices)
{
    assert(mglDrawRangeElements);
    mglDrawRangeElements(mode, start, end, count, type, indices);
}

void TexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels)
{
    assert(mglTexImage3D);
    mglTexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels);
}

void TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels)
{
    assert(mglTexSubImage3D);
    mglTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

void CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    assert(mglCopyTexSubImage3D);
    mglCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
}

// GL_VERSION_1_3

void ActiveTexture(GLenum texture)
{
    assert(mglActiveTexture);
    mglActiveTexture(texture);
}

void SampleCoverage(GLfloat value, GLboolean invert)
{
    assert(mglSampleCoverage);
    mglSampleCoverage(value, invert);
}

void CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data)
{
    assert(mglCompressedTexImage3D);
    mglCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
}

void CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
    assert(mglCompressedTexImage2D);
    mglCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

void CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data)
{
    assert(mglCompressedTexImage1D);
    mglCompressedTexImage1D(target, level, internalformat, width, border, imageSize, data);
}

void CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data)
{
    assert(mglCompressedTexSubImage3D);
    mglCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
}

void CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data)
{
    assert(mglCompressedTexSubImage2D);
    mglCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data)
{
    assert(mglCompressedTexSubImage1D);
    mglCompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data);
}

void GetCompressedTexImage(GLenum target, GLint level, void* img)
{
    assert(mglGetCompressedTexImage);
    mglGetCompressedTexImage(target, level, img);
}

// GL_VERSION_1_4

void BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    assert(mglBlendFuncSeparate);
    mglBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

void MultiDrawArrays(GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount)
{
    assert(mglMultiDrawArrays);
    mglMultiDrawArrays(mode, first, count, drawcount);
}

void MultiDrawElements(GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount)
{
    assert(mglMultiDrawElements);
    mglMultiDrawElements(mode, count, type, indices, drawcount);
}

void PointParameterf(GLenum pname, GLfloat param)
{
    assert(mglPointParameterf);
    mglPointParameterf(pname, param);
}

void PointParameterfv(GLenum pname, const GLfloat* params)
{
    assert(mglPointParameterfv);
    mglPointParameterfv(pname, params);
}

void PointParameteri(GLenum pname, GLint param)
{
    assert(mglPointParameteri);
    mglPointParameteri(pname, param);
}

void PointParameteriv(GLenum pname, const GLint* params)
{
    assert(mglPointParameteriv);
    mglPointParameteriv(pname, params);
}

void BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    assert(mglBlendColor);
    mglBlendColor(red, green, blue, alpha);
}

void BlendEquation(GLenum mode)
{
    assert(mglBlendEquation);
    mglBlendEquation(mode);
}

// GL_VERSION_1_5

void GenQueries(GLsizei n, GLuint* ids)
{
    assert(mglGenQueries);
    mglGenQueries(n, ids);
}

void DeleteQueries(GLsizei n, const GLuint* ids)
{
    assert(mglDeleteQueries);
    mglDeleteQueries(n, ids);
}

GLboolean IsQuery(GLuint id)
{
    assert(mglIsQuery);
    return mglIsQuery(id);
}

void BeginQuery(GLenum target, GLuint id)
{
    assert(mglBeginQuery);
    mglBeginQuery(target, id);
}

void EndQuery(GLenum target)
{
    assert(mglEndQuery);
    mglEndQuery(target);
}

void GetQueryiv(GLenum target, GLenum pname, GLint* params)
{
    assert(mglGetQueryiv);
    mglGetQueryiv(target, pname, params);
}

void GetQueryObjectiv(GLuint id, GLenum pname, GLint* params)
{
    assert(mglGetQueryObjectiv);
    mglGetQueryObjectiv(id, pname, params);
}

void GetQueryObjectuiv(GLuint id, GLenum pname, GLuint* params)
{
    assert(mglGetQueryObjectuiv);
    mglGetQueryObjectuiv(id, pname, params);
}

void BindBuffer(GLenum target, GLuint buffer)
{
    assert(mglBindBuffer);
    mglBindBuffer(target, buffer);
}

void DeleteBuffers(GLsizei n, const GLuint* buffers)
{
    assert(mglDeleteBuffers);
    mglDeleteBuffers(n, buffers);
}

void GenBuffers(GLsizei n, GLuint* buffers)
{
    assert(mglGenBuffers);
    mglGenBuffers(n, buffers);
}

GLboolean IsBuffer(GLuint buffer)
{
    assert(mglIsBuffer);
    return mglIsBuffer(buffer);
}

void BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
    assert(mglBufferData);
    mglBufferData(target, size, data, usage);
}

void BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
    assert(mglBufferSubData);
    mglBufferSubData(target, offset, size, data);
}

void GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void* data)
{
    assert(mglGetBufferSubData);
    mglGetBufferSubData(target, offset, size, data);
}

void* MapBuffer(GLenum target, GLenum access)
{
    assert(mglMapBuffer);
    return mglMapBuffer(target, access);
}

GLboolean UnmapBuffer(GLenum target)
{
    assert(mglUnmapBuffer);
    return mglUnmapBuffer(target);
}

void GetBufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    assert(mglGetBufferParameteriv);
    mglGetBufferParameteriv(target, pname, params);
}

void GetBufferPointerv(GLenum target, GLenum pname, void** params)
{
    assert(mglGetBufferPointerv);
    mglGetBufferPointerv(target, pname, params);
}

// GL_VERSION_2_0

void BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    assert(mglBlendEquationSeparate);
    mglBlendEquationSeparate(modeRGB, modeAlpha);
}

void DrawBuffers(GLsizei n, const GLenum* bufs)
{
    assert(mglDrawBuffers);
    mglDrawBuffers(n, bufs);
}

void StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
    assert(mglStencilOpSeparate);
    mglStencilOpSeparate(face, sfail, dpfail, dppass);
}

void StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    assert(mglStencilFuncSeparate);
    mglStencilFuncSeparate(face, func, ref, mask);
}

void StencilMaskSeparate(GLenum face, GLuint mask)
{
    assert(mglStencilMaskSeparate);
    mglStencilMaskSeparate(face, mask);
}

void AttachShader(GLuint program, GLuint shader)
{
    assert(mglAttachShader);
    mglAttachShader(program, shader);
}

void BindAttribLocation(GLuint program, GLuint index, const GLchar* name)
{
    assert(mglBindAttribLocation);
    mglBindAttribLocation(program, index, name);
}

void CompileShader(GLuint shader)
{
    assert(mglCompileShader);
    mglCompileShader(shader);
}

GLuint CreateProgram()
{
    assert(mglCreateProgram);
    return mglCreateProgram();
}

GLuint CreateShader(GLenum type)
{
    assert(mglCreateShader);
    return mglCreateShader(type);
}

void DeleteProgram(GLuint program)
{
    assert(mglDeleteProgram);
    mglDeleteProgram(program);
}

void DeleteShader(GLuint shader)
{
    assert(mglDeleteShader);
    mglDeleteShader(shader);
}

void DetachShader(GLuint program, GLuint shader)
{
    assert(mglDetachShader);
    mglDetachShader(program, shader);
}

void DisableVertexAttribArray(GLuint index)
{
    assert(mglDisableVertexAttribArray);
    mglDisableVertexAttribArray(index);
}

void EnableVertexAttribArray(GLuint index)
{
    assert(mglEnableVertexAttribArray);
    mglEnableVertexAttribArray(index);
}

void GetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
    assert(mglGetActiveAttrib);
    mglGetActiveAttrib(program, index, bufSize, length, size, type, name);
}

void GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
    assert(mglGetActiveUniform);
    mglGetActiveUniform(program, index, bufSize, length, size, type, name);
}

void GetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders)
{
    assert(mglGetAttachedShaders);
    mglGetAttachedShaders(program, maxCount, count, shaders);
}

GLint GetAttribLocation(GLuint program, const GLchar* name)
{
    assert(mglGetAttribLocation);
    return mglGetAttribLocation(program, name);
}

void GetProgramiv(GLuint program, GLenum pname, GLint* params)
{
    assert(mglGetProgramiv);
    mglGetProgramiv(program, pname, params);
}

void GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
    assert(mglGetProgramInfoLog);
    mglGetProgramInfoLog(program, bufSize, length, infoLog);
}

void GetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
    assert(mglGetShaderiv);
    mglGetShaderiv(shader, pname, params);
}

void GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
    assert(mglGetShaderInfoLog);
    mglGetShaderInfoLog(shader, bufSize, length, infoLog);
}

void GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source)
{
    assert(mglGetShaderSource);
    mglGetShaderSource(shader, bufSize, length, source);
}

GLint GetUniformLocation(GLuint program, const GLchar* name)
{
    assert(mglGetUniformLocation);
    return mglGetUniformLocation(program, name);
}

void GetUniformfv(GLuint program, GLint location, GLfloat* params)
{
    assert(mglGetUniformfv);
    mglGetUniformfv(program, location, params);
}

void GetUniformiv(GLuint program, GLint location, GLint* params)
{
    assert(mglGetUniformiv);
    mglGetUniformiv(program, location, params);
}

void GetVertexAttribdv(GLuint index, GLenum pname, GLdouble* params)
{
    assert(mglGetVertexAttribdv);
    mglGetVertexAttribdv(index, pname, params);
}

void GetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
    assert(mglGetVertexAttribfv);
    mglGetVertexAttribfv(index, pname, params);
}

void GetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
    assert(mglGetVertexAttribiv);
    mglGetVertexAttribiv(index, pname, params);
}

void GetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer)
{
    assert(mglGetVertexAttribPointerv);
    mglGetVertexAttribPointerv(index, pname, pointer);
}

GLboolean IsProgram(GLuint program)
{
    assert(mglIsProgram);
    return mglIsProgram(program);
}

GLboolean IsShader(GLuint shader)
{
    assert(mglIsShader);
    return mglIsShader(shader);
}

void LinkProgram(GLuint program)
{
    assert(mglLinkProgram);
    mglLinkProgram(program);
}

void ShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
    assert(mglShaderSource);
    mglShaderSource(shader, count, string, length);
}

void UseProgram(GLuint program)
{
    assert(mglUseProgram);
    mglUseProgram(program);
}

void Uniform1f(GLint location, GLfloat v0)
{
    assert(mglUniform1f);
    mglUniform1f(location, v0);
}

void Uniform2f(GLint location, GLfloat v0, GLfloat v1)
{
    assert(mglUniform2f);
    mglUniform2f(location, v0, v1);
}

void Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    assert(mglUniform3f);
    mglUniform3f(location, v0, v1, v2);
}

void Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    assert(mglUniform4f);
    mglUniform4f(location, v0, v1, v2, v3);
}

void Uniform1i(GLint location, GLint v0)
{
    assert(mglUniform1i);
    mglUniform1i(location, v0);
}

void Uniform2i(GLint location, GLint v0, GLint v1)
{
    assert(mglUniform2i);
    mglUniform2i(location, v0, v1);
}

void Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
    assert(mglUniform3i);
    mglUniform3i(location, v0, v1, v2);
}

void Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    assert(mglUniform4i);
    mglUniform4i(location, v0, v1, v2, v3);
}

void Uniform1fv(GLint location, GLsizei count, const GLfloat* value)
{
    assert(mglUniform1fv);
    mglUniform1fv(location, count, value);
}

void Uniform2fv(GLint location, GLsizei count, const GLfloat* value)
{
    assert(mglUniform2fv);
    mglUniform2fv(location, count, value);
}

void Uniform3fv(GLint location, GLsizei count, const GLfloat* value)
{
    assert(mglUniform3fv);
    mglUniform3fv(location, count, value);
}

void Uniform4fv(GLint location, GLsizei count, const GLfloat* value)
{
    assert(mglUniform4fv);
    mglUniform4fv(location, count, value);
}

void Uniform1iv(GLint location, GLsizei count, const GLint* value)
{
    assert(mglUniform1iv);
    mglUniform1iv(location, count, value);
}

void Uniform2iv(GLint location, GLsizei count, const GLint* value)
{
    assert(mglUniform2iv);
    mglUniform2iv(location, count, value);
}

void Uniform3iv(GLint location, GLsizei count, const GLint* value)
{
    assert(mglUniform3iv);
    mglUniform3iv(location, count, value);
}

void Uniform4iv(GLint location, GLsizei count, const GLint* value)
{
    assert(mglUniform4iv);
    mglUniform4iv(location, count, value);
}

void UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix2fv);
    mglUniformMatrix2fv(location, count, transpose, value);
}

void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix3fv);
    mglUniformMatrix3fv(location, count, transpose, value);
}

void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix4fv);
    mglUniformMatrix4fv(location, count, transpose, value);
}

void ValidateProgram(GLuint program)
{
    assert(mglValidateProgram);
    mglValidateProgram(program);
}

void VertexAttrib1d(GLuint index, GLdouble x)
{
    assert(mglVertexAttrib1d);
    mglVertexAttrib1d(index, x);
}

void VertexAttrib1dv(GLuint index, const GLdouble* v)
{
    assert(mglVertexAttrib1dv);
    mglVertexAttrib1dv(index, v);
}

void VertexAttrib1f(GLuint index, GLfloat x)
{
    assert(mglVertexAttrib1f);
    mglVertexAttrib1f(index, x);
}

void VertexAttrib1fv(GLuint index, const GLfloat* v)
{
    assert(mglVertexAttrib1fv);
    mglVertexAttrib1fv(index, v);
}

void VertexAttrib1s(GLuint index, GLshort x)
{
    assert(mglVertexAttrib1s);
    mglVertexAttrib1s(index, x);
}

void VertexAttrib1sv(GLuint index, const GLshort* v)
{
    assert(mglVertexAttrib1sv);
    mglVertexAttrib1sv(index, v);
}

void VertexAttrib2d(GLuint index, GLdouble x, GLdouble y)
{
    assert(mglVertexAttrib2d);
    mglVertexAttrib2d(index, x, y);
}

void VertexAttrib2dv(GLuint index, const GLdouble* v)
{
    assert(mglVertexAttrib2dv);
    mglVertexAttrib2dv(index, v);
}

void VertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
{
    assert(mglVertexAttrib2f);
    mglVertexAttrib2f(index, x, y);
}

void VertexAttrib2fv(GLuint index, const GLfloat* v)
{
    assert(mglVertexAttrib2fv);
    mglVertexAttrib2fv(index, v);
}

void VertexAttrib2s(GLuint index, GLshort x, GLshort y)
{
    assert(mglVertexAttrib2s);
    mglVertexAttrib2s(index, x, y);
}

void VertexAttrib2sv(GLuint index, const GLshort* v)
{
    assert(mglVertexAttrib2sv);
    mglVertexAttrib2sv(index, v);
}

void VertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z)
{
    assert(mglVertexAttrib3d);
    mglVertexAttrib3d(index, x, y, z);
}

void VertexAttrib3dv(GLuint index, const GLdouble* v)
{
    assert(mglVertexAttrib3dv);
    mglVertexAttrib3dv(index, v);
}

void VertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
    assert(mglVertexAttrib3f);
    mglVertexAttrib3f(index, x, y, z);
}

void VertexAttrib3fv(GLuint index, const GLfloat* v)
{
    assert(mglVertexAttrib3fv);
    mglVertexAttrib3fv(index, v);
}

void VertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z)
{
    assert(mglVertexAttrib3s);
    mglVertexAttrib3s(index, x, y, z);
}

void VertexAttrib3sv(GLuint index, const GLshort* v)
{
    assert(mglVertexAttrib3sv);
    mglVertexAttrib3sv(index, v);
}

void VertexAttrib4Nbv(GLuint index, const GLbyte* v)
{
    assert(mglVertexAttrib4Nbv);
    mglVertexAttrib4Nbv(index, v);
}

void VertexAttrib4Niv(GLuint index, const GLint* v)
{
    assert(mglVertexAttrib4Niv);
    mglVertexAttrib4Niv(index, v);
}

void VertexAttrib4Nsv(GLuint index, const GLshort* v)
{
    assert(mglVertexAttrib4Nsv);
    mglVertexAttrib4Nsv(index, v);
}

void VertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)
{
    assert(mglVertexAttrib4Nub);
    mglVertexAttrib4Nub(index, x, y, z, w);
}

void VertexAttrib4Nubv(GLuint index, const GLubyte* v)
{
    assert(mglVertexAttrib4Nubv);
    mglVertexAttrib4Nubv(index, v);
}

void VertexAttrib4Nuiv(GLuint index, const GLuint* v)
{
    assert(mglVertexAttrib4Nuiv);
    mglVertexAttrib4Nuiv(index, v);
}

void VertexAttrib4Nusv(GLuint index, const GLushort* v)
{
    assert(mglVertexAttrib4Nusv);
    mglVertexAttrib4Nusv(index, v);
}

void VertexAttrib4bv(GLuint index, const GLbyte* v)
{
    assert(mglVertexAttrib4bv);
    mglVertexAttrib4bv(index, v);
}

void VertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
    assert(mglVertexAttrib4d);
    mglVertexAttrib4d(index, x, y, z, w);
}

void VertexAttrib4dv(GLuint index, const GLdouble* v)
{
    assert(mglVertexAttrib4dv);
    mglVertexAttrib4dv(index, v);
}

void VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    assert(mglVertexAttrib4f);
    mglVertexAttrib4f(index, x, y, z, w);
}

void VertexAttrib4fv(GLuint index, const GLfloat* v)
{
    assert(mglVertexAttrib4fv);
    mglVertexAttrib4fv(index, v);
}

void VertexAttrib4iv(GLuint index, const GLint* v)
{
    assert(mglVertexAttrib4iv);
    mglVertexAttrib4iv(index, v);
}

void VertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)
{
    assert(mglVertexAttrib4s);
    mglVertexAttrib4s(index, x, y, z, w);
}

void VertexAttrib4sv(GLuint index, const GLshort* v)
{
    assert(mglVertexAttrib4sv);
    mglVertexAttrib4sv(index, v);
}

void VertexAttrib4ubv(GLuint index, const GLubyte* v)
{
    assert(mglVertexAttrib4ubv);
    mglVertexAttrib4ubv(index, v);
}

void VertexAttrib4uiv(GLuint index, const GLuint* v)
{
    assert(mglVertexAttrib4uiv);
    mglVertexAttrib4uiv(index, v);
}

void VertexAttrib4usv(GLuint index, const GLushort* v)
{
    assert(mglVertexAttrib4usv);
    mglVertexAttrib4usv(index, v);
}

void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
    assert(mglVertexAttribPointer);
    mglVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

// GL_VERSION_2_1

void UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix2x3fv);
    mglUniformMatrix2x3fv(location, count, transpose, value);
}

void UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix3x2fv);
    mglUniformMatrix3x2fv(location, count, transpose, value);
}

void UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix2x4fv);
    mglUniformMatrix2x4fv(location, count, transpose, value);
}

void UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix4x2fv);
    mglUniformMatrix4x2fv(location, count, transpose, value);
}

void UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix3x4fv);
    mglUniformMatrix3x4fv(location, count, transpose, value);
}

void UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
    assert(mglUniformMatrix4x3fv);
    mglUniformMatrix4x3fv(location, count, transpose, value);
}

// GL_VERSION_3_0

void ColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{
    assert(mglColorMaski);
    mglColorMaski(index, r, g, b, a);
}

void GetBooleani_v(GLenum target, GLuint index, GLboolean* data)
{
    assert(mglGetBooleani_v);
    mglGetBooleani_v(target, index, data);
}

void GetIntegeri_v(GLenum target, GLuint index, GLint* data)
{
    assert(mglGetIntegeri_v);
    mglGetIntegeri_v(target, index, data);
}

void Enablei(GLenum target, GLuint index)
{
    assert(mglEnablei);
    mglEnablei(target, index);
}

void Disablei(GLenum target, GLuint index)
{
    assert(mglDisablei);
    mglDisablei(target, index);
}

GLboolean IsEnabledi(GLenum target, GLuint index)
{
    assert(mglIsEnabledi);
    return mglIsEnabledi(target, index);
}

void BeginTransformFeedback(GLenum primitiveMode)
{
    assert(mglBeginTransformFeedback);
    mglBeginTransformFeedback(primitiveMode);
}

void EndTransformFeedback()
{
    assert(mglEndTransformFeedback);
    mglEndTransformFeedback();
}

void BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
    assert(mglBindBufferRange);
    mglBindBufferRange(target, index, buffer, offset, size);
}

void BindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
    assert(mglBindBufferBase);
    mglBindBufferBase(target, index, buffer);
}

void TransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode)
{
    assert(mglTransformFeedbackVaryings);
    mglTransformFeedbackVaryings(program, count, varyings, bufferMode);
}

void GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name)
{
    assert(mglGetTransformFeedbackVarying);
    mglGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
}

void ClampColor(GLenum target, GLenum clamp)
{
    assert(mglClampColor);
    mglClampColor(target, clamp);
}

void BeginConditionalRender(GLuint id, GLenum mode)
{
    assert(mglBeginConditionalRender);
    mglBeginConditionalRender(id, mode);
}

void EndConditionalRender()
{
    assert(mglEndConditionalRender);
    mglEndConditionalRender();
}

void VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer)
{
    assert(mglVertexAttribIPointer);
    mglVertexAttribIPointer(index, size, type, stride, pointer);
}

void GetVertexAttribIiv(GLuint index, GLenum pname, GLint* params)
{
    assert(mglGetVertexAttribIiv);
    mglGetVertexAttribIiv(index, pname, params);
}

void GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint* params)
{
    assert(mglGetVertexAttribIuiv);
    mglGetVertexAttribIuiv(index, pname, params);
}

void VertexAttribI1i(GLuint index, GLint x)
{
    assert(mglVertexAttribI1i);
    mglVertexAttribI1i(index, x);
}

void VertexAttribI2i(GLuint index, GLint x, GLint y)
{
    assert(mglVertexAttribI2i);
    mglVertexAttribI2i(index, x, y);
}

void VertexAttribI3i(GLuint index, GLint x, GLint y, GLint z)
{
    assert(mglVertexAttribI3i);
    mglVertexAttribI3i(index, x, y, z);
}

void VertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
{
    assert(mglVertexAttribI4i);
    mglVertexAttribI4i(index, x, y, z, w);
}

void VertexAttribI1ui(GLuint index, GLuint x)
{
    assert(mglVertexAttribI1ui);
    mglVertexAttribI1ui(index, x);
}

void VertexAttribI2ui(GLuint index, GLuint x, GLuint y)
{
    assert(mglVertexAttribI2ui);
    mglVertexAttribI2ui(index, x, y);
}

void VertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z)
{
    assert(mglVertexAttribI3ui);
    mglVertexAttribI3ui(index, x, y, z);
}

void VertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
{
    assert(mglVertexAttribI4ui);
    mglVertexAttribI4ui(index, x, y, z, w);
}

void VertexAttribI1iv(GLuint index, const GLint* v)
{
    assert(mglVertexAttribI1iv);
    mglVertexAttribI1iv(index, v);
}

void VertexAttribI2iv(GLuint index, const GLint* v)
{
    assert(mglVertexAttribI2iv);
    mglVertexAttribI2iv(index, v);
}

void VertexAttribI3iv(GLuint index, const GLint* v)
{
    assert(mglVertexAttribI3iv);
    mglVertexAttribI3iv(index, v);
}

void VertexAttribI4iv(GLuint index, const GLint* v)
{
    assert(mglVertexAttribI4iv);
    mglVertexAttribI4iv(index, v);
}

void VertexAttribI1uiv(GLuint index, const GLuint* v)
{
    assert(mglVertexAttribI1uiv);
    mglVertexAttribI1uiv(index, v);
}

void VertexAttribI2uiv(GLuint index, const GLuint* v)
{
    assert(mglVertexAttribI2uiv);
    mglVertexAttribI2uiv(index, v);
}

void VertexAttribI3uiv(GLuint index, const GLuint* v)
{
    assert(mglVertexAttribI3uiv);
    mglVertexAttribI3uiv(index, v);
}

void VertexAttribI4uiv(GLuint index, const GLuint* v)
{
    assert(mglVertexAttribI4uiv);
    mglVertexAttribI4uiv(index, v);
}

void VertexAttribI4bv(GLuint index, const GLbyte* v)
{
    assert(mglVertexAttribI4bv);
    mglVertexAttribI4bv(index, v);
}

void VertexAttribI4sv(GLuint index, const GLshort* v)
{
    assert(mglVertexAttribI4sv);
    mglVertexAttribI4sv(index, v);
}

void VertexAttribI4ubv(GLuint index, const GLubyte* v)
{
    assert(mglVertexAttribI4ubv);
    mglVertexAttribI4ubv(index, v);
}

void VertexAttribI4usv(GLuint index, const GLushort* v)
{
    assert(mglVertexAttribI4usv);
    mglVertexAttribI4usv(index, v);
}

void GetUniformuiv(GLuint program, GLint location, GLuint* params)
{
    assert(mglGetUniformuiv);
    mglGetUniformuiv(program, location, params);
}

void BindFragDataLocation(GLuint program, GLuint color, const GLchar* name)
{
    assert(mglBindFragDataLocation);
    mglBindFragDataLocation(program, color, name);
}

GLint GetFragDataLocation(GLuint program, const GLchar* name)
{
    assert(mglGetFragDataLocation);
    return mglGetFragDataLocation(program, name);
}

void Uniform1ui(GLint location, GLuint v0)
{
    assert(mglUniform1ui);
    mglUniform1ui(location, v0);
}

void Uniform2ui(GLint location, GLuint v0, GLuint v1)
{
    assert(mglUniform2ui);
    mglUniform2ui(location, v0, v1);
}

void Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
    assert(mglUniform3ui);
    mglUniform3ui(location, v0, v1, v2);
}

void Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
    assert(mglUniform4ui);
    mglUniform4ui(location, v0, v1, v2, v3);
}

void Uniform1uiv(GLint location, GLsizei count, const GLuint* value)
{
    assert(mglUniform1uiv);
    mglUniform1uiv(location, count, value);
}

void Uniform2uiv(GLint location, GLsizei count, const GLuint* value)
{
    assert(mglUniform2uiv);
    mglUniform2uiv(location, count, value);
}

void Uniform3uiv(GLint location, GLsizei count, const GLuint* value)
{
    assert(mglUniform3uiv);
    mglUniform3uiv(location, count, value);
}

void Uniform4uiv(GLint location, GLsizei count, const GLuint* value)
{
    assert(mglUniform4uiv);
    mglUniform4uiv(location, count, value);
}

void TexParameterIiv(GLenum target, GLenum pname, const GLint* params)
{
    assert(mglTexParameterIiv);
    mglTexParameterIiv(target, pname, params);
}

void TexParameterIuiv(GLenum target, GLenum pname, const GLuint* params)
{
    assert(mglTexParameterIuiv);
    mglTexParameterIuiv(target, pname, params);
}

void GetTexParameterIiv(GLenum target, GLenum pname, GLint* params)
{
    assert(mglGetTexParameterIiv);
    mglGetTexParameterIiv(target, pname, params);
}

void GetTexParameterIuiv(GLenum target, GLenum pname, GLuint* params)
{
    assert(mglGetTexParameterIuiv);
    mglGetTexParameterIuiv(target, pname, params);
}

void ClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint* value)
{
    assert(mglClearBufferiv);
    mglClearBufferiv(buffer, drawbuffer, value);
}

void ClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint* value)
{
    assert(mglClearBufferuiv);
    mglClearBufferuiv(buffer, drawbuffer, value);
}

void ClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value)
{
    assert(mglClearBufferfv);
    mglClearBufferfv(buffer, drawbuffer, value);
}

void ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
{
    assert(mglClearBufferfi);
    mglClearBufferfi(buffer, drawbuffer, depth, stencil);
}

const GLubyte* GetStringi(GLenum name, GLuint index)
{
    assert(mglGetStringi);
    return mglGetStringi(name, index);
}

GLboolean IsRenderbuffer(GLuint renderbuffer)
{
    assert(mglIsRenderbuffer);
    return mglIsRenderbuffer(renderbuffer);
}

void BindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    assert(mglBindRenderbuffer);
    mglBindRenderbuffer(target, renderbuffer);
}

void DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
    assert(mglDeleteRenderbuffers);
    mglDeleteRenderbuffers(n, renderbuffers);
}

void GenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
    assert(mglGenRenderbuffers);
    mglGenRenderbuffers(n, renderbuffers);
}

void RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    assert(mglRenderbufferStorage);
    mglRenderbufferStorage(target, internalformat, width, height);
}

void GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
    assert(mglGetRenderbufferParameteriv);
    mglGetRenderbufferParameteriv(target, pname, params);
}

GLboolean IsFramebuffer(GLuint framebuffer)
{
    assert(mglIsFramebuffer);
    return mglIsFramebuffer(framebuffer);
}

void BindFramebuffer(GLenum target, GLuint framebuffer)
{
    assert(mglBindFramebuffer);
    mglBindFramebuffer(target, framebuffer);
}

void DeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
    assert(mglDeleteFramebuffers);
    mglDeleteFramebuffers(n, framebuffers);
}

void GenFramebuffers(GLsizei n, GLuint* framebuffers)
{
    assert(mglGenFramebuffers);
    mglGenFramebuffers(n, framebuffers);
}

GLenum CheckFramebufferStatus(GLenum target)
{
    assert(mglCheckFramebufferStatus);
    return mglCheckFramebufferStatus(target);
}

void FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    assert(mglFramebufferTexture1D);
    mglFramebufferTexture1D(target, attachment, textarget, texture, level);
}

void FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    assert(mglFramebufferTexture2D);
    mglFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
    assert(mglFramebufferTexture3D);
    mglFramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
}

void FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    assert(mglFramebufferRenderbuffer);
    mglFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
    assert(mglGetFramebufferAttachmentParameteriv);
    mglGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

void GenerateMipmap(GLenum target)
{
    assert(mglGenerateMipmap);
    mglGenerateMipmap(target);
}

void BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
    assert(mglBlitFramebuffer);
    mglBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

void RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
    assert(mglRenderbufferStorageMultisample);
    mglRenderbufferStorageMultisample(target, samples, internalformat, width, height);
}

void FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
{
    assert(mglFramebufferTextureLayer);
    mglFramebufferTextureLayer(target, attachment, texture, level, layer);
}

void* MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
    assert(mglMapBufferRange);
    return mglMapBufferRange(target, offset, length, access);
}

void FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
{
    assert(mglFlushMappedBufferRange);
    mglFlushMappedBufferRange(target, offset, length);
}

void BindVertexArray(GLuint array)
{
    assert(mglBindVertexArray);
    mglBindVertexArray(array);
}

void DeleteVertexArrays(GLsizei n, const GLuint* arrays)
{
    assert(mglDeleteVertexArrays);
    mglDeleteVertexArrays(n, arrays);
}

void GenVertexArrays(GLsizei n, GLuint* arrays)
{
    assert(mglGenVertexArrays);
    mglGenVertexArrays(n, arrays);
}

GLboolean IsVertexArray(GLuint array)
{
    assert(mglIsVertexArray);
    return mglIsVertexArray(array);
}

// GL_VERSION_3_1

void DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount)
{
    assert(mglDrawArraysInstanced);
    mglDrawArraysInstanced(mode, first, count, instancecount);
}

void DrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount)
{
    assert(mglDrawElementsInstanced);
    mglDrawElementsInstanced(mode, count, type, indices, instancecount);
}

void TexBuffer(GLenum target, GLenum internalformat, GLuint buffer)
{
    assert(mglTexBuffer);
    mglTexBuffer(target, internalformat, buffer);
}

void PrimitiveRestartIndex(GLuint index)
{
    assert(mglPrimitiveRestartIndex);
    mglPrimitiveRestartIndex(index);
}

void CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
    assert(mglCopyBufferSubData);
    mglCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
}

void GetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices)
{
    assert(mglGetUniformIndices);
    mglGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
}

void GetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params)
{
    assert(mglGetActiveUniformsiv);
    mglGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
}

void GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName)
{
    assert(mglGetActiveUniformName);
    mglGetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
}

GLuint GetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName)
{
    assert(mglGetUniformBlockIndex);
    return mglGetUniformBlockIndex(program, uniformBlockName);
}

void GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params)
{
    assert(mglGetActiveUniformBlockiv);
    mglGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
}

void GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName)
{
    assert(mglGetActiveUniformBlockName);
    mglGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
}

void UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
    assert(mglUniformBlockBinding);
    mglUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

// GL_VERSION_3_2

void DrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex)
{
    assert(mglDrawElementsBaseVertex);
    mglDrawElementsBaseVertex(mode, count, type, indices, basevertex);
}

void DrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices, GLint basevertex)
{
    assert(mglDrawRangeElementsBaseVertex);
    mglDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
}

void DrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex)
{
    assert(mglDrawElementsInstancedBaseVertex);
    mglDrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex);
}

void MultiDrawElementsBaseVertex(GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount, const GLint* basevertex)
{
    assert(mglMultiDrawElementsBaseVertex);
    mglMultiDrawElementsBaseVertex(mode, count, type, indices, drawcount, basevertex);
}

void ProvokingVertex(GLenum mode)
{
    assert(mglProvokingVertex);
    mglProvokingVertex(mode);
}

GLsync FenceSync(GLenum condition, GLbitfield flags)
{
    assert(mglFenceSync);
    return mglFenceSync(condition, flags);
}

GLboolean IsSync(GLsync sync)
{
    assert(mglIsSync);
    return mglIsSync(sync);
}

void DeleteSync(GLsync sync)
{
    assert(mglDeleteSync);
    mglDeleteSync(sync);
}

GLenum ClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
    assert(mglClientWaitSync);
    return mglClientWaitSync(sync, flags, timeout);
}

void WaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
    assert(mglWaitSync);
    mglWaitSync(sync, flags, timeout);
}

void GetInteger64v(GLenum pname, GLint64* data)
{
    assert(mglGetInteger64v);
    mglGetInteger64v(pname, data);
}

void GetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values)
{
    assert(mglGetSynciv);
    mglGetSynciv(sync, pname, bufSize, length, values);
}

void GetInteger64i_v(GLenum target, GLuint index, GLint64* data)
{
    assert(mglGetInteger64i_v);
    mglGetInteger64i_v(target, index, data);
}

void GetBufferParameteri64v(GLenum target, GLenum pname, GLint64* params)
{
    assert(mglGetBufferParameteri64v);
    mglGetBufferParameteri64v(target, pname, params);
}

void FramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level)
{
    assert(mglFramebufferTexture);
    mglFramebufferTexture(target, attachment, texture, level);
}

void TexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
{
    assert(mglTexImage2DMultisample);
    mglTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
}

void TexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
{
    assert(mglTexImage3DMultisample);
    mglTexImage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
}

void GetMultisamplefv(GLenum pname, GLuint index, GLfloat* val)
{
    assert(mglGetMultisamplefv);
    mglGetMultisamplefv(pname, index, val);
}

void SampleMaski(GLuint maskNumber, GLbitfield mask)
{
    assert(mglSampleMaski);
    mglSampleMaski(maskNumber, mask);
}

// GL_VERSION_3_3

void BindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar* name)
{
    assert(mglBindFragDataLocationIndexed);
    mglBindFragDataLocationIndexed(program, colorNumber, index, name);
}

GLint GetFragDataIndex(GLuint program, const GLchar* name)
{
    assert(mglGetFragDataIndex);
    return mglGetFragDataIndex(program, name);
}

void GenSamplers(GLsizei count, GLuint* samplers)
{
    assert(mglGenSamplers);
    mglGenSamplers(count, samplers);
}

void DeleteSamplers(GLsizei count, const GLuint* samplers)
{
    assert(mglDeleteSamplers);
    mglDeleteSamplers(count, samplers);
}

GLboolean IsSampler(GLuint sampler)
{
    assert(mglIsSampler);
    return mglIsSampler(sampler);
}

void BindSampler(GLuint unit, GLuint sampler)
{
    assert(mglBindSampler);
    mglBindSampler(unit, sampler);
}

void SamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
    assert(mglSamplerParameteri);
    mglSamplerParameteri(sampler, pname, param);
}

void SamplerParameteriv(GLuint sampler, GLenum pname, const GLint* param)
{
    assert(mglSamplerParameteriv);
    mglSamplerParameteriv(sampler, pname, param);
}

void SamplerParameterf(GLuint sampler, GLenum pname, GLfloat param)
{
    assert(mglSamplerParameterf);
    mglSamplerParameterf(sampler, pname, param);
}

void SamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat* param)
{
    assert(mglSamplerParameterfv);
    mglSamplerParameterfv(sampler, pname, param);
}

void SamplerParameterIiv(GLuint sampler, GLenum pname, const GLint* param)
{
    assert(mglSamplerParameterIiv);
    mglSamplerParameterIiv(sampler, pname, param);
}

void SamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint* param)
{
    assert(mglSamplerParameterIuiv);
    mglSamplerParameterIuiv(sampler, pname, param);
}

void GetSamplerParameteriv(GLuint sampler, GLenum pname, GLint* params)
{
    assert(mglGetSamplerParameteriv);
    mglGetSamplerParameteriv(sampler, pname, params);
}

void GetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint* params)
{
    assert(mglGetSamplerParameterIiv);
    mglGetSamplerParameterIiv(sampler, pname, params);
}

void GetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat* params)
{
    assert(mglGetSamplerParameterfv);
    mglGetSamplerParameterfv(sampler, pname, params);
}

void GetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint* params)
{
    assert(mglGetSamplerParameterIuiv);
    mglGetSamplerParameterIuiv(sampler, pname, params);
}

void QueryCounter(GLuint id, GLenum target)
{
    assert(mglQueryCounter);
    mglQueryCounter(id, target);
}

void GetQueryObjecti64v(GLuint id, GLenum pname, GLint64* params)
{
    assert(mglGetQueryObjecti64v);
    mglGetQueryObjecti64v(id, pname, params);
}

void GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64* params)
{
    assert(mglGetQueryObjectui64v);
    mglGetQueryObjectui64v(id, pname, params);
}

void VertexAttribDivisor(GLuint index, GLuint divisor)
{
    assert(mglVertexAttribDivisor);
    mglVertexAttribDivisor(index, divisor);
}

void VertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
    assert(mglVertexAttribP1ui);
    mglVertexAttribP1ui(index, type, normalized, value);
}

void VertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value)
{
    assert(mglVertexAttribP1uiv);
    mglVertexAttribP1uiv(index, type, normalized, value);
}

void VertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
    assert(mglVertexAttribP2ui);
    mglVertexAttribP2ui(index, type, normalized, value);
}

void VertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value)
{
    assert(mglVertexAttribP2uiv);
    mglVertexAttribP2uiv(index, type, normalized, value);
}

void VertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
    assert(mglVertexAttribP3ui);
    mglVertexAttribP3ui(index, type, normalized, value);
}

void VertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value)
{
    assert(mglVertexAttribP3uiv);
    mglVertexAttribP3uiv(index, type, normalized, value);
}

void VertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value)
{
    assert(mglVertexAttribP4ui);
    mglVertexAttribP4ui(index, type, normalized, value);
}

void VertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint* value)
{
    assert(mglVertexAttribP4uiv);
    mglVertexAttribP4uiv(index, type, normalized, value);
}

// GL_ARB_debug_output

void DebugMessageControlARB(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled)
{
    assert(mglDebugMessageControlARB);
    mglDebugMessageControlARB(source, type, severity, count, ids, enabled);
}

void DebugMessageInsertARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf)
{
    assert(mglDebugMessageInsertARB);
    mglDebugMessageInsertARB(source, type, id, severity, length, buf);
}

void DebugMessageCallbackARB(GLDEBUGPROCARB callback, const void* userParam)
{
    assert(mglDebugMessageCallbackARB);
    mglDebugMessageCallbackARB(callback, userParam);
}

GLuint GetDebugMessageLogARB(GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog)
{
    assert(mglGetDebugMessageLogARB);
    return mglGetDebugMessageLogARB(count, bufSize, sources, types, ids, severities, lengths, messageLog);
}

} // namespace gl
