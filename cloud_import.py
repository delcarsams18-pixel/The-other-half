import unreal

def import_skeletal_fbx():
    # 1. Set up the automated import task
    task = unreal.AssetImportTask()
    task.filename = "C:/CloudPath/RawImports/Walking_21_0keg.fbx"
    task.destination_path = "/Game/Animations/Lonzo"
    task.save = True
    task.automated = True

    # 2. Configure specifically for skeletal mesh
    options = unreal.FbxImportUI()
    options.import_mesh = True
    options.as_skeletal_mesh = True
    options.import_as_skeletal = True
    options.automated_import_settings = True

    task.options = options

    # 3. Run the import pipeline headlessly
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    print("Cloud Import Complete: FBX converted to skeletal mesh")

import_skeletal_fbx()
