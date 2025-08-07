// glad.h - arquivo gigante, mas vou te passar uma versão mínima para OpenGL 3.3 Core

#ifndef __GLAD_H_
#define __GLAD_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void* GLADloadproc;

int gladLoadGLLoader(GLADloadproc load);

#ifdef __cplusplus
}
#endif

#endif
