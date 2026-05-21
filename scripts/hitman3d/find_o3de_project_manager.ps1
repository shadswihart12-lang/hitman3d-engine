# Finds O3DE Project Manager (o3de.exe or O3DEProjectManager.exe)
param(
    [Parameter(Mandatory = $true)][string]$EngineRoot
)

$engineRoot = (Resolve-Path $EngineRoot).Path
$candidates = New-Object System.Collections.Generic.List[string]

function Add-IfExists([string]$path) {
    if ($path -and (Test-Path -LiteralPath $path)) {
        [void]$candidates.Add((Resolve-Path -LiteralPath $path).Path)
    }
}

# Built from YOUR fork (Hitman branding after editor build)
Add-IfExists (Join-Path $engineRoot 'build\windows\bin\profile\Editor\O3DEProjectManager.exe')
Add-IfExists (Join-Path $engineRoot 'build\windows\bin\profile\Default\o3de.exe')
Add-IfExists (Join-Path $engineRoot 'build\windows\bin\profile\o3de.exe')
Add-IfExists (Join-Path $engineRoot 'build\bin\profile\Editor\O3DEProjectManager.exe')
Add-IfExists (Join-Path $engineRoot 'build\bin\profile\Default\o3de.exe')

# Official O3DE installer (O3DE branding — use until your fork is built)
$searchRoots = @(
    ${env:ProgramFiles},
    ${env:ProgramFiles(x86)},
    ${env:LOCALAPPDATA},
    'C:\O3DE',
    'C:\Amazon\O3DE'
) | Where-Object { $_ -and (Test-Path $_) }

$names = @('o3de.exe', 'O3DEProjectManager.exe')
foreach ($root in $searchRoots) {
    foreach ($name in $names) {
        try {
            Get-ChildItem -Path $root -Filter $name -Recurse -ErrorAction SilentlyContinue -Depth 8 |
                Where-Object { $_.FullName -match '\\bin\\.*\\(profile|Editor|Default)\\' } |
                ForEach-Object { [void]$candidates.Add($_.FullName) }
        } catch {}
    }
}

# PATH
foreach ($name in $names) {
    $cmd = Get-Command $name -ErrorAction SilentlyContinue
    if ($cmd) { [void]$candidates.Add($cmd.Source) }
}

$unique = $candidates | Select-Object -Unique
if ($unique.Count -eq 0) {
    Write-Error "Could not find o3de.exe or O3DEProjectManager.exe. Install O3DE from https://o3de.org/download/ or build your engine."
    exit 1
}

# Prefer fork build, then shortest path (usually official install)
$pick = $unique | Where-Object { $_ -like "$engineRoot*" } | Select-Object -First 1
if (-not $pick) { $pick = $unique | Select-Object -First 1 }

Write-Output $pick
