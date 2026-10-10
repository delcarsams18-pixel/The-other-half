# THE OTHER HALF

Unreal Engine 5.5.4 third-person shooter. Built on unrealcloud.io.

## Animation Pipeline: FBX to Embedded C++

Since we can't run the Unreal Editor (no hardware), animations are baked from FBX to C++ headers.

### How it works

1. Parse the Mixamo FBX for bone hierarchy, skin weights, and animation curves
2. Bake each animation frame to vertex positions
3. Generate a C++ header with the baked frames
4. Game cycles frames at runtime (flip-book style)

### Baking a walk animation

```bash
# From the repo root, with the FBX in workspace/user/files/
python3 ~/workspace/bake_walk5.py
# Output: /tmp/LonzoWalkAnim.h

# Copy to the project
cp /tmp/LonzoWalkAnim.h Source/TOH/Public/
```

### What gets baked

- 65 Mixamo bones (LimbNode hierarchy)
- 52 skin clusters (vertex weights)
- 315 animation curves
- 32 keyframes per walk cycle
- 4,622 vertices per frame

### Adding to a character

```cpp
// In TOHCharacter.h
#include "LonzoWalkAnim.h"
UPROPERTY()
TArray<UProceduralMeshComponent*> WalkFrameMeshes;

// In TOHCharacter.cpp constructor
for (int32 i = 0; i < LONZO_WALK_FRAMES; i++)
{
    // Create one mesh per frame, toggle visibility in Tick
}
```

### Project structure

- `Source/TOH/` - C++ game code
- `Content/Animations/` - Raw FBX files (not imported, for reference)
- `Content/Models/` - GLB source models

## Building

Push to GitHub, build on [unrealcloud.io](https://unrealcloud.io). Windows + Android targets.

## Company

Pulse Plait Tech
