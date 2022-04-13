# Zips the plugin, keeping only required and valid files for Marketplace distribution
import os
import zipfile

def zip(dest):
    to_include = (
        "Config",
        "Resources",
        "Source",
        "LICENSE",
        "LabStreamingLayer.uplugin"
    )
    with zipfile.ZipFile(dest, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for root, dirs, files in os.walk('.'):
            for file in files:
                relpath = os.path.relpath(os.path.join(root, file), '.')
                if not relpath.startswith(to_include):
                    continue
                # Removing lslver.exe
                if relpath.endswith(".exe"):
                    continue
                zipf.write(os.path.join(root, file), relpath)

zip('UE5-LSL.zip')
