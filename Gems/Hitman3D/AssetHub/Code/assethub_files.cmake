set(FILES
    Include/Hitman3D/AssetHub/AssetHubBus.h
    Include/Hitman3D/AssetHub/AssetMetadata.h
    Include/Hitman3D/AssetHub/IAssetSource.h

    Source/AssetHubSystemComponent.h
    Source/AssetHubSystemComponent.cpp

    Source/Sources/SketchfabSource.h
    Source/Sources/SketchfabSource.cpp
    Source/Sources/MegascansSource.h
    Source/Sources/MegascansSource.cpp
    Source/Sources/OpenGameArtSource.h
    Source/Sources/OpenGameArtSource.cpp
    Source/Sources/FabSource.h
    Source/Sources/FabSource.cpp

    Source/Pipeline/AssetImporter.h
    Source/Pipeline/AssetImporter.cpp

    Source/Auth/CredentialStore.h
    Source/Auth/CredentialStore_Windows.cpp
)
