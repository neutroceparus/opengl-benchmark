//	Copyright (c) 2025 Paul Jankowski

#ifndef OPENGL_IMPORTS
#define OPENGL_IMPORTS

/*
	mini-library to facilitate things related to
	importing OpenGL functions
	especially Windows - specific
*/
#include "global_defines.h"

#include <stdlib.h>
#include <string.h>

#if defined _WIN32
		
	//order important
	#include "KHR\khrplatform.h"
	#include "GL\glcorearb.h"
	#include "GL\glext.h"
	#include "gl_compatibility_patch.h"

	//wgl
	#include "GL\wgl.h"
	#include "GL\wglext.h"
	
	#if defined OPENGL_IMPORTS_SUPPORT_SDL
		#include "SDL2\SDL.h"
		#include "SDL2\SDL_syswm.h"
	#endif
	
	#include <windows.h>
	#include <winnt.h>

#else
	
	#include <dlfcn.h>

	//order important
	#include "KHR/khrplatform.h"
	#include "GL/glcorearb.h"
	#include "GL/glext.h"
	#include <X11/Xlib.h>
	#define GLX_GLXEXT_LEGACY // to ensure glxext from project
	#include <GL/glx.h>
	#undef GLX_GLXEXT_LEGACY
	#include "GL/glxext.h"
	#include "gl_compatibility_patch.h"

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_syswm.h>

#endif

#include "simplest_log.h"
#include "utils.h"

#if !(defined _WIN32)
	#include "system_info.h"
	#include "sdl_functions_imports.h"
#endif

#define NUM_OF_GL_IMPORTS 85

#if defined _WIN32
#define NUM_OF_WGL_IMPORTS 3
#else
#define NUM_OF_GLX_IMPORTS 2
#endif

#define OPENGL_IMPORTS_STATE_INVALID 0
#define OPENGL_IMPORTS_STATE_EMPTY 1
#define OPENGL_IMPORTS_STATE_VALID 2

// 3.0
#define FROM_THIS_VERSION_IS_IN_INTEGER 30

// uncomment to enable it
//#define PIXEL_FORMAT_DIAGNOSTICS

// libGLX - moved here
// Ubuntu 22.04/24.04
#define LIBGLX_NAME "libGLX.so.0"

#ifdef __cplusplus
extern "C" {
#endif

/*
		opengl_imports
			
			its main struct to hold functions pointers
			and organizational data
*/
struct opengl_imports
{
	int state;
	int total_present;
	
	//check this before using function
	int * presence_table;
	char ** names_for_presence_checking;
	
	// version (in this imports)
	int opengl_version;
	
	// general extensions
	int num_of_general_extensions;
	char ** general_extensions_names;
	int * general_extensions_functions_presence_if_applicable;
	
	// wgl and glx extensions
#if defined _WIN32
	int num_of_wgl_extensions;
	char ** wgl_extensions_names;
	int * wgl_extensions_functions_presence_if_applicable;
#else // other cases?
	int num_of_glx_extensions;
	char ** glx_extensions_names;
	int * glx_extensions_functions_presence_if_applicable;
#endif
	
#if defined _WIN32
	//HGLRC
	void * corresponding_win_style_opengl_context;
	
	// This does not work, because device context is highly "volatile"
	// always use wglGetCurrentDC or GetDC (and related)
	//HDC
	//void * corresponding_win_style_device_context;
	
	int corresponding_pixel_format_if_any;
	PIXELFORMATDESCRIPTOR \
		corresponding_pixel_format_descriptor_if_any;
	
	//HMODULE
	void * opengl32_dll_handle;
	int opengl32_dll_loaded;

#else
	
	void * libglx_so_handle;

	PFNGLXGETPROCADDRESSPROC glXGetProcAddress;

#endif
	
#if defined SDL_h_
	SDL_Window * corresponding_sdl_window_if_used;
#endif
	
/*
		1.0
*/
	
	//GLenum / unsigned int, GLfloat (GLclampf) / float
	PFNGLALPHAFUNC glAlphaFunc;
	
	// void (GLenum / unsigned int)
	PFNGLBEGINPROC glBegin;
	
	// void (GLenum / unsigned int, GLenum / unsigned int)
	PFNGLBLENDFUNCPROC glBlendFunc;
	
	//GLuint / unsigned int
	PFNGLCALLLISTPROC glCallList;
	
	// void (GLbitfield mask)
	PFNGLCLEARPROC glClear;
	
	// void (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	PFNGLCLEARCOLORPROC glClearColor;
	
	// 4 times GLfloat / float
	PFNGLCOLOR4FPROC glColor4f;
	
	// void(GLenum / unsigned int);
	PFNGLCULLFACEPROC glCullFace;
	
	//GLuint / unsigned int, GLsizei / int
	PFNGLDELETELISTSPROC glDeleteLists;
	
	// void (GLenum / unsigned int)
	PFNGLDEPTHFUNCPROC glDepthFunc;
	
	//void (GLenum / unsigned int)
	PFNGLDISABLEPROC glDisable;
	
	//void (GLenum / unsigned int)
	PFNGLDRAWBUFFERPROC glDrawBuffer;
	
	// void (GLenum / unsigned int)
	PFNGLREADBUFFERPROC glReadBuffer;
	
	//GLboolean / unsigned char
	PFNGLEDGEFLAGPROC glEdgeFlag;
	
	// void (GLenum / unsigned int cap)
	PFNGLENABLEPROC glEnable;
	
	// void
	PFNGLENDPROC glEnd;
	
	// void
	PFNGLENDLISTPROC glEndList;
	
	// void (void)
	PFNGLFLUSHPROC glFlush;
	
	//GLsizei / int
	PFNGLGENLISTSPROC glGenLists;
	
	// GLenum / unsigned int (void)
	PFNGLGETERRORPROC glGetError;
	
	// void (GLenum / unsigned int, GLint *)
	PFNGLGETINTEGERVPROC glGetIntegerv;
	
	// const GLubyte */ signed char * ()
	PFNGLGETSTRINGPROC glGetString;
	
	//GLenum / unsigned int,
	//GLint / int,
	//GLenum / unsigned int,
	//GLenum / unsigned int,
	//void *
	PFNGLGETTEXIMAGEPROC glGetTexImage;
	
	//GLfloat / float
	PFNGLLINEWIDTHPROC glLineWidth;
	
	// void
	PFNGLLOADIDENTITYPROC glLoadIdentity;
	
	//const GLfloat * / const float *
	PFNGLLOADMATRIXF glLoadMatrixf;
	
	//GLenum / unsigned int
	PFNGLMATRIXMODEPROC glMatrixMode;
	
	//GLuint / unsigned int, GLenum / unsigned int
	PFNGLNEWLIST glNewList;
	
	// 3 times GLfloat / float
	PFNGLNORMAL3FPROC glNormal3f;
	
	//GLenum / unsigned int, GLint / int
	PFNGLPIXELSTOREIPROC glPixelStorei;
	
	// void glPolygonMode (
	//	GLenum / unsigned int face,
	//	GLenum / unsigned int mode
	//	)
	PFNGLPOLYGONMODEPROC glPolygonMode;
	
	//GLint / int,
	//GLint / int,
	//GLsizei / int,
	//GLsizei / int,
	//GLenum / unsigned int,
	//GLenum / unsigned int,
	//void *
	PFNGLREADPIXELSPROC glReadPixels;
	
	//4 times: GLfloat / float
	PFNGLTEXCOORD4FPROC glTexCoord4f;
	
	//GLenum / unsigned int, GLenum / unsigned int, GLfloat / float
	PFNGLTEXENVF glTexEnvf;
	
	//GLenum / unsigned int, GLenum / unsigned int, const GLfloat * / const float *
	PFNGLTEXENVFV glTexEnvfv;
	
	//void (
	//	GLenum / unsigned int target,
	//	GLint / int level,
	//	GLint /int internalformat,
	//	GLsizei / int width,
	//	GLsizei /int height,
	//	GLint / int border,
	//	GLenum / unsigned int format,
	//	GLenum / unsigned int type,
	//	const void *pixels
	//	);
	PFNGLTEXIMAGE2DPROC glTexImage2D;
	
	//	GLenum / unsigned int target,
	//	GLenum / unsigned int pname,
	//	const GLfloat * / const float * params
	PFNGLTEXPARAMETERFVPROC glTexParameterfv;
	
	//void glTexParameteri (
	//	GLenum / unsigned int target,
	//	GLenum / unsigned int pname,
	//	GLint / int param);
	PFNGLTEXPARAMETERIPROC glTexParameteri;
	
	// 4 times GLfloat / float
	PFNGLVERTEX4FPROC glVertex4f;
	
	// void (GLint x, GLint y, GLsizei width, GLsizei height)
	PFNGLVIEWPORTPROC glViewport;
	
/*
		1.1
*/
	
	//void (GLenum \ unsigned int target, GLuint / unsigned int texture)
	PFNGLBINDTEXTUREPROC glBindTexture;
	
	//void(
	//	GLsizei / unsigned int n,
	//	const GLuint * / const unsigned int * textures)
	PFNGLDELETETEXTURESPROC glDeleteTextures;
	
	// void (
	//	GLenum / unsigned int mode,
	//	GLint / int first,
	//	GLsizei / int count
	//	);
	PFNGLDRAWARRAYSPROC glDrawArrays;
	
	// void (
	//	GLenum / unsigned int mode,
	//	GLsizei / int count,
	//	GLenum/ unsigned int type,
	//	const void * indices
	//	);
	PFNGLDRAWELEMENTSPROC glDrawElements;
	
	// void (GLsizei / int n, GLuint * / unsigned int * textures);
	PFNGLGENTEXTURESPROC glGenTextures;
	
/*
		1.3
*/
	
	//void (GLenum / unsigned int texture)
	PFNGLACTIVETEXTUREPROC glActiveTexture;
	
/*
		1.5
*/

	// void (GLenum / unsigned int, GLuint / unsigned int)
	PFNGLBINDBUFFERPROC glBindBuffer;
	
	// void (
	//	GLenum / unsigned int,
	//	GLsizeiptr / (_WIN64: signed long long int, other: signed long int)
	//	see: khrplatform.h,
	//	const void *,
	//	GLenum / unsigned int
	//	)
	PFNGLBUFFERDATAPROC glBufferData;
	
	// void (GLsizei / int n, const GLuint * / const unsigned int * buffers);
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	
	// void (GLsizei / int, GLuint / unsigned int)
	PFNGLGENBUFFERSPROC glGenBuffers;
	
/*
		2.0
*/

	// void (GLuint / unsigned int, GLuint / unsigned int)
	PFNGLATTACHSHADERPROC glAttachShader;
	
	// void (GLuint / unsigned int, GLuint / unsigned int, const GLchar * / const char *)
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
	
	// void (GLuint / unsigned int)
	PFNGLCOMPILESHADERPROC glCompileShader;
	
	// Gluint / unsigned int (void)
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	
	// GLuint / unsigned int (GLenum / unsigned int)
	PFNGLCREATESHADERPROC glCreateShader;
	
	// void (GLuint / unsigned int program)
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	
	// void (GLuint / unsigned int)
	PFNGLDELETESHADERPROC glDeleteShader;
	
	//(GLsizei / int, const GLenum * / const unsigned int *)
	PFNGLDRAWBUFFERSPROC glDrawBuffers;
	
	// void (GLuint / unsigned int)
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	
	// void (
	//	GLuint / unsigned int,
	//	GLsizei / int,
	//	GLsizei * / int *,
	//	GLchar * / char *
	//	);
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	
	// void (
	//	GLuint / unsigned int program,
	//	GLenum / unsigned int pname,
	//	GLint * / int * params);
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	
	// void (
	//	GLuint / unsigned int,
	//	GLsizei / int,
	//	GLsizei * / int *,
	//	GLchar * / char *
	//	);
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	
	// void (GLuint / unsigned int, GLenum / unsigned int, GLint * / int *)
	PFNGLGETSHADERIVPROC glGetShaderiv;
	
	//GLint / int (
	//	GLuint / unsigned int program,
	//	const GLchar * / const char *name
	//	)
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	
	//GLboolean / unsigned char (GLuint / unsigned int)
	PFNGLISPROGRAMPROC glIsProgram;
	
	// void (GLuint / unsigned int)
	PFNGLLINKPROGRAMPROC glLinkProgram;
	
	// void (
	//	GLuint / unsigned int,
	//	GLsizei / int,
	//	wild notation:
	//	const GLchar *const*string / const char * const*,
	//	 // two - level constness !
	//	const GLint * / const int *
	//	)
	PFNGLSHADERSOURCEPROC glShaderSource;
	
	//void (GLint / int location, GLint / int v0);
	PFNGLUNIFORM1IPROC glUniform1i;
	
	//void glUniform3f(
	//	GLint / int location,
	//	GLfloat / float v0,
	//	GLfloat /float v1,
	//	GLfloat / float v2
	//	)
	PFNGLUNIFORM3FPROC glUniform3f;
	
	//void glUniform3fv (
	//	GLint / int location,
	//	GLsizei / int count,
	//	const GLfloat * / const float * value)
	PFNGLUNIFORM3FVPROC glUniform3fv;
	
	//void glUniform4f(
	//	GLint / int location,
	//	GLfloat / float v0,
	//	GLfloat / float v1,
	//	GLfloat / float v2,
	//	GLfloat / float v3
	//	)
	PFNGLUNIFORM4FPROC glUniform4f;
	
	//void(
	//	GLint / int location,
	//	GLsizei / int count,
	//	GLboolean / unsigned char transpose,
	//	const GLfloat * / const float * value
	//	)
	PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
	
	//void(
	//	GLint / int location,
	//	GLsizei / int count,
	//	GLboolean / unsigned char transpose,
	//	const GLfloat * / const float * value
	//	)
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	
	// void (GLuint / unsigned int)
	PFNGLUSEPROGRAMPROC glUseProgram;
	
	// void (GLuint / unsigned int)
	PFNGLVALIDATEPROGRAMPROC glValidateProgram;
	
	// void (
	//	GLuint / unsigned int,
	//	GLint / int,
	//	GLenum / unsigned int,
	//	GLboolean / unsigned char,
	//	GLsizei / int,
	//	const void *
	//	)
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	
/*
		3.0
*/
	
	//void(GLenum / unsigned int, GLuint / unsigned int)
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;

	// void glBindVertexArray (GLuint / unsigned int array);
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	
	// void (GLsizei / int n, const GLuint * / const unsigned int * arrays);
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
	
	//GLenum * 3 / unsigned int * 3, GLuint / unsigned int, GLint / int
	PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
	
	//void (GLenum target)
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	
	//void(GLsizei / int, GLuint * / unsigned int *)
	PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	
	// void glGenVertexArrays (
	//	GLsizei / int n,
	//	GLuint * / unsigned int * arrays
	//	);
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	
	//const GLubyte * / unsigned char * (
	//	GLenum / unsigned int name,
	//	GLuint / unsigned int index
	//	);
	PFNGLGETSTRINGIPROC glGetStringi;
	
/*
		3.1
*/
	
	//void glDrawElementsInstanced(
	//	GLenum / unsigned int mode,
	//	GLsizei / int  count,
	//	GLenum / unsigned int type,
	//	const void * indices,
	//	GLsizei / int instancecount
	//	)
	PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;

	
/*
		GLX functions
*/
#if !(defined _WIN32)

/*
		GLX 1.1
*/
	
	//const char * (
	//Display - see <X11/Xlib.h>,
	//int)
	PFNGLXQUERYEXTENSIONSSTRINGPROC glXQueryExtensionsString;

/*
		extensions functions - GLX
*/

	//see: <GL/glx.h>, <X11/Xlib.h>, "GL/glxext"
	//and others
	//GLXContext(
	//Display *,
	//GLXFBConfig,
	//GLXContext,
	//Bool / int,
	//const int *)
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;

#endif
	
/*
		extensions functions - general extensions
*/
	
	// currently none
	
/*
		extensions functions - WGL
*/
#if defined _WIN32
	
	// const char * (HDC / void *)
	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
	
	// int (void)
	PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;
	
	// win bool
	// BOOL / int (int)
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#endif
};

/*
		opengl_imports_general_init
			
			run this at start of program
			on linux it imports functions
			on windows it makes shadow window
			to check version and functions
*/
int opengl_imports_general_init(
#if defined _WIN32
	void * hinstance_of_app //HINSTANCE
#else
	void
#endif
	);

/*
		opengl_imports_general_deinit
		
			run at end of program
*/
int opengl_imports_general_deinit(void);

/*
		opengl_imports_general_check
			
			it doesnt check everything
*/
int opengl_imports_general_check(void);

/*
		general_opengl_version
			
			returns version checked at initialization
			there are cases that context has
			different version
*/
int general_opengl_version(void);

/*
		opengl_version_this_context
			
			usable on windows - there should be
			same as checked on init, but this
			is used just in case
*/
#if defined _WIN32
int opengl_version_this_context(void);
#endif

/*
		general_opengl_version_string
			
			same as above but string
*/
char * general_opengl_version_string(void);

/*
		opengl_imports_initial_version_check
*/
void opengl_imports_initial_version_check(
#if defined _WIN32
	void * passed_hinstance_of_app //HINSTANCE
#else
	void
#endif
	);

/*
		setup_calloced_or_static_opengl_imports_struct
			
			its mostly for internal use
			
			calloced or static, because relies on some zeros
			for checking
*/
//in Windows gl context must be set already
int setup_calloced_or_static_opengl_imports_struct(
	struct opengl_imports * struct_to_setup
#if defined _WIN32
	,
	int pixel_format_and_gl_context_in_struct_already
#endif
	);

#if !(defined _WIN32)
/*
		fill_extensions_data_in_opengl_imports
			
			this is because different philosophy
			on Windows and Linux
			this function is called once after setup
			of imports on Linux
			(temp context is done inside)
*/
int fill_extensions_data_in_opengl_imports(
	struct opengl_imports * imports
	);
#endif

/*
		deinit_opengl_imports_only_internal_freeing
*/	
int deinit_opengl_imports_only_internal_freeing(
	struct opengl_imports * struct_to_deinit
	);

/*
		is_gl_function_present_by_name
			
			on win its in current context
*/
int is_gl_function_present_by_name(
	const char * name_to_check
	);

/*
		all_gl_functions_present_by_names
			
			pass char * arguments
*/
int all_gl_functions_present_by_names(
	int num_of_funcs_to_check,
	...
	);

/*
		is_gl_extension_present_by_name
			
			it checks wgl and glx also
*/
int is_gl_extension_present_by_name(
	const char * name_to_check
	);

#if defined _WIN32
/*
		is_gl_extension_present_by_name_this_context
			
			it checks wgl also
*/
int is_gl_extension_present_by_name_this_context(
	const char * name_to_check
	);
#endif

/*
		OPENGL_IMPORTS_PREPARE_ENTRY
			
			macro for internal use
			used when preparing imports struct
*/
#define OPENGL_IMPORTS_PREPARE_ENTRY(\
	name, name_in_double_commas) \
	*( \
		(*struct_to_setup).names_for_presence_checking \
		+ \
		index_of_cur_import \
		) \
		= \
			(char *) \
				calloc( \
					strlen(name_in_double_commas) + 1, \
					sizeof(char) \
					); \
	\
	if( \
		*( \
			(*struct_to_setup).names_for_presence_checking \
			+ \
			index_of_cur_import \
			) \
		== \
		NULL \
		) \
	{ \
		importing_proceed = MY_FALSE; \
		simplest_log( \
			"setup_calloced_or_static" \
			"_opengl_imports_struct " \
			"cannot calloc %s", \
			"name"\
			); \
		cannot_calloc_between_imports = \
			index_of_cur_import; \
		importing_proceed = MY_FALSE; \
	} \
	else \
	{ \
		strcpy( \
			*( \
				(*struct_to_setup).names_for_presence_checking \
				+ \
				index_of_cur_import \
			), \
			name_in_double_commas \
			); \
		ULTRA_STRICT_LOG( \
			"setup_calloced_or_static" \
			"_opengl_imports_struct " \
			"calloc %s ok", \
			name_in_double_commas) \
	} \
	 \
	if ((*struct_to_setup).name != NULL) \
	{ \
		*((*struct_to_setup).presence_table + index_of_cur_import) = \
			MY_TRUE; \
		ULTRA_STRICT_LOG("Seems like entry %s is present", \
			name_in_double_commas); \
		(*struct_to_setup).total_present += 1; \
	} \
	else \
	{ \
		*((*struct_to_setup).presence_table + index_of_cur_import) = \
			MY_FALSE; \
		ULTRA_STRICT_LOG("Seems like entry %s is NOT present", \
			name_in_double_commas); \
	} \
	 \
	if(index_of_cur_import == num_of_imports) \
	{\
		simplest_log( \
			"setup_calloced_or_static" \
			"_opengl_imports_struct " \
			"more imports than defined" \
			); \
		importing_proceed = MY_FALSE; \
	} \
	 \
	index_of_cur_import += 1;

/*
	OpenGL functions
*/

/*
		1.0
*/

//GLenum / unsigned int, GLfloat (GLclampf) / float
void glAlphaFunc(GLenum func, GLfloat ref);

// void (GLenum / unsigned int)
void glBegin(GLenum mode);

// void (GLenum / unsigned int, GLenum / unsigned int)
void glBlendFunc (GLenum sfactor, GLenum dfactor);

//GLuint / unsigned int
void glCallList(GLuint list);

// void (unsigned int)
void glClear (GLbitfield mask);
		
// void (float, float, float, float)
void glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

// 4 times GLfloat / float
void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

// void(GLenum / unsigned int);
void glCullFace (GLenum mode);

//GLuint / unsigned int, GLsizei / int
void glDeleteLists(GLuint list, GLsizei range);

// void (GLenum / unsigned int)
void glDepthFunc (GLenum func);

//void (GLenum / unsigned int)
void glDisable (GLenum cap);

//void (GLenum / unsigned int)
void glDrawBuffer (GLenum buf);

// void (GLenum / unsigned int)
void glReadBuffer (GLenum src);

//GLboolean / unsigned char
void glEdgeFlag(GLboolean flag);

// void (GLenum / unsigned int cap)
void glEnable (GLenum cap);

// void
void glEnd(void);

// void
void glEndList(void);

// void (void)
void glFlush(void);

//GLsizei / int
GLuint glGenLists(GLsizei range);

// unsigned int (void)
GLenum glGetError (void);

// void (unsigned int, int)
void glGetIntegerv (GLenum pname, GLint *data);

// const unsigned char * (unsigned int)
const GLubyte * glGetString (GLenum name);

//GLenum / unsigned int,
//GLint / int,
//GLenum / unsigned int,
//GLenum / unsigned int,
//void *
void glGetTexImage(
	GLenum target,
	GLint level,
	GLenum format,
	GLenum type,
	void *pixels
	);

//GLfloat / float
void glLineWidth(GLfloat width);

// void
void glLoadIdentity(void);

//const GLfloat * / const float *
void glLoadMatrixf(const GLfloat *m);

//GLenum / unsigned int
void glMatrixMode(GLenum mode);

//GLuint / unsigned int, GLenum / unsigned int
void glNewList(GLuint list, GLenum mode);

// 3 times GLfloat / float
void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);

//GLenum / unsigned int, GLint / int
void glPixelStorei(GLenum pname, GLint param);

// void glPolygonMode (
//	GLenum / unsigned int face,
//	GLenum / unsigned int mode
//	)
void glPolygonMode (GLenum face, GLenum mode);

//GLint / int,
//GLint / int,
//GLsizei / int,
//GLsizei / int,
//GLenum / unsigned int,
//GLenum / unsigned int,
//void *
void glReadPixels(
	GLint x,
	GLint y,
	GLsizei width,
	GLsizei height,
	GLenum format,
	GLenum type,
	void *pixels
	);

//4 times: GLfloat / float
void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);

//GLenum / unsigned int, GLenum / unsigned int, GLfloat / float
void glTexEnvf(GLenum target, GLenum pname, GLfloat param);

//GLenum / unsigned int, GLenum / unsigned int, const GLfloat * / const float *
void glTexEnvfv(GLenum target, GLenum pname, const GLfloat * params);

//void (
//	GLenum / unsigned int target,
//	GLint / int level,
//	GLint /int internalformat,
//	GLsizei / int width,
//	GLsizei /int height,
//	GLint / int border,
//	GLenum / unsigned int format,
//	GLenum / unsigned int type,
//	const void *pixels
//	);
void glTexImage2D(
	GLenum target,
	GLint level,
	GLint internalformat,
	GLsizei width,
	GLsizei height,
	GLint border,
	GLenum format,
	GLenum type,
	const void *pixels
	);

//	GLenum / unsigned int target,
//	GLenum / unsigned int pname,
//	const GLfloat * / const float * params
void glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
	
//void glTexParameteri (
//	GLenum / unsigned int target,
//	GLenum / unsigned int pname,
//	GLint / int param);
void glTexParameteri (GLenum target, GLenum pname, GLint param);

// 4 times GLfloat / float
void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

// void (int, int, int, int)
void glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
	
/*
		1.1
*/
		
//void (GLenum \ unsigned int target, GLuint / unsigned int texture)
void glBindTexture (GLenum target, GLuint texture);

//void(
//	GLsizei / unsigned int n,
//	const GLuint * / const unsigned int * textures)
void glDeleteTextures (GLsizei n, const GLuint *textures);

// void (
//	GLenum / unsigned int mode,
//	GLint / int first,
//	GLsizei / int count
//	);
void glDrawArrays (GLenum mode, GLint first, GLsizei count);

// void (
//	GLenum / unsigned int mode,
//	GLsizei / int count,
//	GLenum/ unsigned int type,
//	const void * indices
//	);
void glDrawElements(
	GLenum mode,
	GLsizei count,
	GLenum type,
	const void *indices
	);

// void (GLsizei / int n, GLuint * / unsigned int * textures);
void glGenTextures (GLsizei n, GLuint *textures);
	
/*
		1.3
*/

//void (GLenum / unsigned int texture)
void glActiveTexture (GLenum texture);

	
/*
		1.5
*/

// (unsigned int, unsigned int)
void glBindBuffer(GLenum target, GLuint buffer);

// void (
//	GLenum / unsigned int,
//	GLsizeiptr / (_WIN64: signed long long int, other: signed long int)
//	see: khrplatform.h,
//	const void *,
//	GLenum / unsigned int
//	)
void glBufferData(
	GLenum target,
	GLsizeiptr size,
	const void *data,
	GLenum usage
	);
	
// void (GLsizei / int n, const GLuint * / const unsigned int * buffers);
void glDeleteBuffers (GLsizei n, const GLuint *buffers);

// void (GLsizei / int, GLuint / unsigned int)
void glGenBuffers(GLsizei n, GLuint *buffers);
	
/*
		2.0
*/

// void (GLuint / unsigned int, GLuint / unsigned int)
void glAttachShader (GLuint program, GLuint shader);

// void (GLuint / unsigned int, GLuint / unsigned int, const GLchar * / const char *)
void glBindAttribLocation (GLuint program, GLuint index, const GLchar *name);

// void (GLuint / unsigned int)
void glCompileShader (GLuint shader);

// Gluint / unsigned int (void)
GLuint glCreateProgram (void);

// GLuint / unsigned int (GLenum / unsigned int)
GLuint glCreateShader (GLenum type);

// void (GLuint / unsigned int program)
void glDeleteProgram (GLuint program);

// void (GLuint / unsigned int)
void glDeleteShader (GLuint shader);

//(GLsizei / int, const GLenum * / const unsigned int *)
void glDrawBuffers(GLsizei n, const GLenum *bufs);

// void (GLuint / unsigned int)
void glEnableVertexAttribArray (GLuint index);

// void (
//	GLuint / unsigned int,
//	GLsizei / int,
//	GLsizei * / int *,
//	GLchar * / char *
//	);
void glGetProgramInfoLog(
	GLuint program,
	GLsizei bufSize,
	GLsizei *length,
	GLchar *infoLog
	);
	
// void (
//	GLuint / unsigned int program,
//	GLenum / unsigned int pname,
//	GLint * / int * params);
void glGetProgramiv (GLuint program, GLenum pname, GLint *params);

// void (
//	GLuint / unsigned int,
//	GLsizei / int,
//	GLsizei * / int *,
//	GLchar * / char *
//	);
void glGetShaderInfoLog (
	GLuint shader,
	GLsizei bufSize,
	GLsizei *length,
	GLchar *infoLog
	);
	
// void (GLuint / unsigned int, GLenum / unsigned int, GLint * / int *)
void glGetShaderiv (GLuint shader, GLenum pname, GLint *params);

//GLint / int (
//	GLuint / unsigned int program,
//	const GLchar * / const char *name
//	)
GLint glGetUniformLocation (GLuint program, const GLchar *name);

//GLboolean / unsigned char (GLuint / unsigned int)
GLboolean glIsProgram(GLuint program);

// void (GLuint / unsigned int);
void glLinkProgram (GLuint program);

// void (
//	GLuint / unsigned int,
//	GLsizei / int,
//	wild notation:
//	const GLchar *const*string / const char * const*,
//	 // two - level constness !
//	const GLint * / const int *
//	)
void glShaderSource(
	GLuint shader,
	GLsizei count,
	const GLchar *const*string, // two - level constness !
	const GLint *length
	);
	
//void (GLint / int location, GLint / int v0)
void glUniform1i (GLint location, GLint v0);

//void glUniform3f(
//	GLint / int location,
//	GLfloat / float v0,
//	GLfloat /float v1,
//	GLfloat / float v2
//	)
void glUniform3f(
	GLint location,
	GLfloat v0,
	GLfloat v1,
	GLfloat v2
	);
	
//void glUniform3fv (
//	GLint / int location,
//	GLsizei / int count,
//	const GLfloat * / const float * value)
void glUniform3fv (GLint location, GLsizei count, const GLfloat *value);
	
//void glUniform4f(
//	GLint / int location,
//	GLfloat / float v0,
//	GLfloat / float v1,
//	GLfloat / float v2,
//	GLfloat / float v3
//	)
void glUniform4f(
	GLint location,
	GLfloat v0,
	GLfloat v1,
	GLfloat v2,
	GLfloat v3
	);
	
//void(
//	GLint / int location,
//	GLsizei / int count,
//	GLboolean / unsigned char transpose,
//	const GLfloat * / const float * value
//	)
void glUniformMatrix3fv(
	GLint location,
	GLsizei count,
	GLboolean transpose,
	const GLfloat *value
	);
	
//void(
//	GLint / int location,
//	GLsizei / int count,
//	GLboolean / unsigned char transpose,
//	const GLfloat * / const float * value
//	)
void glUniformMatrix4fv(
	GLint location,
	GLsizei count,
	GLboolean transpose,
	const GLfloat *value
	);
	
// void (GLuint / unsigned int)
void glUseProgram (GLuint program);

// void (GLuint / unsigned int)
void glValidateProgram (GLuint program);

// void (
//	GLuint / unsigned int,
//	GLint / int,
//	GLenum / unsigned int,
//	GLboolean / unsigned char,
//	GLsizei / int,
//	const void *
//	)
void glVertexAttribPointer (
	GLuint index,
	GLint size,
	GLenum type,
	GLboolean normalized,
	GLsizei stride,
	const void *pointer
	);
	
/*
		3.0
*/

//void(GLenum / unsigned int, GLuint / unsigned int)
void glBindFramebuffer(GLenum target, GLuint framebuffer);

// void glBindVertexArray (GLuint / unsigned int array);
void glBindVertexArray (GLuint array);

// void (GLsizei / int n, const GLuint * / const unsigned int * arrays);
void glDeleteVertexArrays (GLsizei n, const GLuint *arrays);

//GLenum * 3 / unsigned int * 3, GLuint / unsigned int, GLint / int
void glFramebufferTexture2D(
	GLenum target,
	GLenum attachment,
	GLenum textarget,
	GLuint texture,
	GLint level
	);

//void glGenerateMipmap (GLenum / unsigned int target)
void glGenerateMipmap (GLenum target);

//void(GLsizei / int, GLuint * / unsigned int *)
void glGenFramebuffers(GLsizei n, GLuint *framebuffers);

// void (
//	GLsizei / int n,
//	GLuint * / unsigned int * arrays
//	);
void glGenVertexArrays (GLsizei n, GLuint *arrays);

//const GLubyte * / unsigned char * (
//	GLenum / unsigned int name,
//	GLuint / unsigned int index
//	);
const GLubyte * glGetStringi (GLenum name, GLuint index);

/*
		3.1
*/
	
//void glDrawElementsInstanced(
//	GLenum / unsigned int mode,
//	GLsizei / int  count,
//	GLenum / unsigned int type,
//	const void * indices,
//	GLsizei / int instancecount
//	)
void glDrawElementsInstanced(
	GLenum mode,
	GLsizei count,
	GLenum type,
	const void *indices,
	GLsizei instancecount
	);

/*
		GLX functions
*/
#if !(defined _WIN32)

/*
		GLX 1.1
*/
	
//const char * (
//Display - see <X11/Xlib.h>,
//int)
const char * glXQueryExtensionsString(Display *dpy, int screen);

/*
		extensions functions - GLX
*/

//see: <GL/glx.h>, <X11/Xlib.h>, "GL/glxext"
//and others
//GLXContext(
//Display *,
//GLXFBConfig / XID / unsigned long,
//GLXContext,
//Bool / int,
//const int *)
GLXContext glXCreateContextAttribsARB(
	Display *dpy,
	GLXFBConfig config,
	GLXContext share_context,
	Bool direct,
	const int *attrib_list
	);

#endif

#if defined _WIN32
/*
	wgl functions
	administrative - organizational
	gl context, device context getting functions etc.
	this is only bare minimum on windows
*/
//administrative
// void * WINAPI void *
typedef HGLRC (__stdcall *wglCreateContext_win_adm_type)(HDC);
//WINBOOL void *
typedef int (__stdcall *wglDeleteContext_win_adm_type)(HGLRC);
//WINBOOL void * void *
typedef int (__stdcall *wglMakeCurrent_win_adm_type)(HDC,HGLRC);
//typedef INT_PTR (WINAPI *PROC) (); (const char *)
typedef PROC (__stdcall *wglGetProcAddress_win_adm_type)(LPCSTR);
// void * WINAPI (void);
typedef HGLRC (__stdcall *wglGetCurrentContext_win_adm_type)(void);
// void * WINAPI (void);
typedef HDC (__stdcall *wglGetCurrentDC_win_adm_type)(void);

#define OPENGL_WIN_ADMINISTRATIVE_STATE_INVALID 0
#define OPENGL_WIN_ADMINISTRATIVE_STATE_HANDLE_ONLY 1
#define OPENGL_WIN_ADMINISTRATIVE_STATE_VALID 2

/*
	struct for storing administrative wgl functions
*/
struct opengl_win_administrative
{
	int state;
	
	void * opengl32_dll_handle;
	int opengl32_dll_loaded;
	
	wglCreateContext_win_adm_type \
		wglCreateContext_win_adm_storage;
	wglDeleteContext_win_adm_type \
		wglDeleteContext_win_adm_storage;
	wglMakeCurrent_win_adm_type \
		wglMakeCurrent_win_adm_storage;
	wglGetProcAddress_win_adm_type \
		wglGetProcAddress_win_adm_storage;
	wglGetCurrentContext_win_adm_type \
		wglGetCurrentContext_win_adm_storage;
	wglGetCurrentDC_win_adm_type \
		wglGetCurrentDC_win_adm_storage;
	
};

// void * (void *)
HGLRC wglCreateContext_win_adm(HDC device_handle);
// int (void *)
int wglDeleteContext_win_adm(HGLRC opengl_context_handle);
// int (void *, void *)
int wglMakeCurrent_win_adm(
	HDC device_handle,
	HGLRC opengl_context_handle
	);
//typedef INT_PTR (WINAPI *PROC) (); (const char *)
PROC wglGetProcAddress_win_adm(LPCSTR func_name);
//void *
HGLRC wglGetCurrentContext_win_adm(void);
//void *
HDC wglGetCurrentDC_win_adm(void);

/*
	
	WGL_EXT_FUNC_INTERNAL_CALL
		
		macro for repeatable code
		in wgl extensions functions
		
		in this case there is no orphan comma
	
*/
#define WGL_EXT_FUNC_INTERNAL_CALL( \
	what_return_on_fail, \
	extension_name_double_quotes, \
	function_name, \
	function_name_double_quotes, \
	... \
	) \
	\
	int index_of_cur_win_gl_context = \
		index_of_win_gl_context_this_thread(); \
	\
	if(index_of_cur_win_gl_context >= 0) \
	{ \
		if( \
			is_gl_extension_present_by_name( \
				extension_name_double_quotes \
				) \
			== \
			MY_TRUE \
			) \
		{ \
			if( \
				(**( \
					opengl_imports_table + \
						index_of_cur_win_gl_context \
					)). \
						function_name \
				!= \
				NULL \
				) \
			{ \
				return \
					(**( \
						opengl_imports_table + \
							index_of_cur_win_gl_context \
						)). \
						function_name(__VA_ARGS__); \
			} \
			else \
			{ \
				simplest_log( \
					function_name_double_quotes \
					" cannot, because " \
					"no function" \
					); \
				return what_return_on_fail; \
			} \
		} \
		simplest_log( \
			function_name_double_quotes \
			" cannot, because no " \
			extension_name_double_quotes \
			); \
		return what_return_on_fail; \
	} \
	else \
	{ \
		simplest_log( \
			function_name_double_quotes \
			" cannot, because " \
			"prerequisites not met, " \
			); \
		return what_return_on_fail; \
	}

/*
		extensions functions - WGL
*/

// const char * (HDC / void *)
const char * wglGetExtensionsStringARB(HDC hdc);

// int (void)
int wglGetSwapIntervalEXT(void);

// win bool
// BOOL / int (int)
BOOL wglSwapIntervalEXT(int interval);

/*
		init_gl_context_win_style_this_thread
			
			most important function on windows
			summarizes proces - making context
			and importing functions
			
			currently its only legacy context
*/
int init_gl_context_win_style_this_thread(
	void * /*HWND*/ window_handle,
	int use_device_context_instead,
	void * /*HDC*/ device_handle_if_supplied
	);

/*
		init_imports_for_sdl_style_context_win_this_thread
			
			same as above but works with
			sdl style gl context
			
			sdl decides if its modern or legacy
			context
*/
#if defined SDL_h_
int init_imports_for_sdl_style_context_win_this_thread(
	SDL_GLContext context_for_check
	);
#endif

/*
		deinit_gl_context_win_style_this_thread
		
			currently only legacy context
*/
int deinit_gl_context_win_style_this_thread(
	int use_gl_context_instead_not_recommended,
	void * /*HGLRC*/ gl_context_if_supplied
	);

/*
		deinit_imports_for_sdl_style_context_win_this_thread
*/
#if defined SDL_h_
int deinit_imports_for_sdl_style_context_win_this_thread(
	SDL_GLContext context_for_check
	);
#endif

/*
		index_of_imports_win_gl_context_by_hglrc
			
			organizational, because they are in
			dynamic table
*/
int index_of_imports_win_gl_context_by_hglrc(
	void * /*HGLRC*/ gl_context_to_search_for
	);

/*
		index_of_win_gl_context_this_thread
			
			here determined which set of imports
			to use
*/
// no way to implicitly pass HWND
int index_of_win_gl_context_this_thread(void);

/*
		opengl_win_adm_general_check
*/
int opengl_win_adm_general_check(void);

/*
		brutal_func_name_by_index_and_pos_in_table
			
			for usage when prerequisites checked earlier, like in:
			is_gl_function_present_by_name
*/
const char * brutal_func_name_by_index_and_pos_in_table(
	int index,
	int pos_in_table
	);

/*
		brutal_presence_by_index_and_pos_in_table
			
			same as above
*/
int brutal_presence_by_index_and_pos_in_table(
	int index,
	int pos_in_table
	);

#endif//_WIN32

#ifdef __cplusplus
}
#endif

#endif //OPENGL_IMPORTS