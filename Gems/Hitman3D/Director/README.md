# Hitman 3d — Director

The **Director** is the AI brain of Hitman 3d by jss. It turns natural language into engine actions, making game development possible for anyone — no code required.

## What it does

Type plain English commands and Director translates them into real engine operations:

| You say | Director does |
|---|---|
| "create a box" | Creates a new entity named "box" |
| "add a light at 0 10 0 intensity 2000" | Places a point light at that position |
| "move 'Player' to 5 0 5" | Repositions the named entity |
| "create level 'MyFirstLevel'" | Creates a new level |
| "generate a script that makes the player jump" | Produces a Lua behavior script |
| "undo" | Reverses the last action |
| "help" | Shows what Director can do |

## Architecture

```
DirectorBus (public EBus API)
    │
    ▼
DirectorSystemComponent
    ├── IntentParser        (NL → ParsedIntent)
    ├── EntityAction        (create, delete, move, scale, rotate, lights, prefabs)
    ├── SceneAction         (levels, skybox)
    ├── ScriptAction        (Lua generation, materials)
    └── TerrainAction       (terrain create/modify)

DirectorWidget (Qt editor panel — conversational UI)
```

## Future roadmap

- **LLM backend**: Replace keyword parser with a hosted LLM for richer understanding
- **Voice input**: Speak commands instead of typing
- **Visual preview**: Show a ghost preview of changes before committing
- **Script Canvas integration**: Generate visual scripts from descriptions
- **AssetHub bridge**: "Find me a tree on Sketchfab and place it here"
