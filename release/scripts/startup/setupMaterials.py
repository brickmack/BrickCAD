import bpy

def createMaterial():
    mat = bpy.data.materials.new(name="Material")
    mat.use_nodes = True
    principled = PrincipledBSDFWrapper(mat, is_readonly=False)
    principled.base_color = (0.8, 0.8, 0.5)
    
    return mat

print("CREATING MATERIALS CREATING MATERIALS CREATING MATERIALS CREATING MATERIALS CREATING MATERIALS CREATING MATERIALS")
createMaterial()
