import unreal

asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
all_assets = asset_registry.get_assets_by_path(
    "/Game/Animations/Pangxie", recursive=True
)

for asset_data in all_assets:
    old_name = str(asset_data.asset_name)
    if old_name.startswith("Pangxie"):
        new_name = old_name.replace("Pangxie", "Crab", 1)
        old_path = asset_data.package_name
        new_path = str(old_path).replace(old_name, new_name)
        unreal.EditorAssetLibrary.rename_asset(str(old_path), new_path)
        print(f"Renamed: {old_name} → {new_name}")

print("Done!")
