set(INC
    ../include
    ../../blenfont
    ../../blenkernel
    ../../blenlib
    ../../gpu
    ../../imbuf
    ../../makesdna
    ../../makesrna
    ../../windowmanager
    ../../../../intern/guardedalloc
)
 
set(INC_SYS
    ${GLEW_INCLUDE_PATH}
)
 
set(SRC
    space_partslist.c
)

set(LIB
  bf_blenkernel
  bf_blenlib
)
 
blender_add_lib(bf_editor_space_partslist "${SRC}" "${INC}" "${INC_SYS}" "${LIB}")
