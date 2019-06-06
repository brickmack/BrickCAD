set(INC
  ../include
  ../../blenfont
  ../../blenkernel
  ../../blenlib
  ../../blentranslation
  ../../gpu
  ../../makesdna
  ../../makesrna
  ../../windowmanager
  ../../../../intern/glew-mx
  ../../../../intern/guardedalloc
)
 
set(INC_SYS
    ${GLEW_INCLUDE_PATH}
)
 
set(SRC
    space_partslist.c
    text_autocomplete.c
    text_draw.c
    text_format.c
    text_format_lua.c
    text_format_osl.c
    text_format_pov.c
    text_format_pov_ini.c
    text_format_py.c
    text_header.c
    text_ops.c
    text_undo.c

    text_format.h
    text_intern.h
)

set(LIB
  bf_blenkernel
  bf_blenlib
)
 
blender_add_lib(bf_editor_space_partslist "${SRC}" "${INC}" "${INC_SYS}" "${LIB}")
