param(
    [string]$EngineRoot
)

$goodEngine = (Resolve-Path $EngineRoot -ErrorAction SilentlyContinue).Path
$searchRoots = @(
    [Environment]::GetFolderPath('Desktop'),
    [Environment]::GetFolderPath('CommonDesktopDirectory'),
    "$env:APPDATA\Microsoft\Windows\Start Menu\Programs"
)

$shell = New-Object -ComObject WScript.Shell
$found = @()

foreach ($root in $searchRoots) {
    if (-not (Test-Path $root)) { continue }
    Get-ChildItem -Path $root -Filter '*.lnk' -Recurse -ErrorAction SilentlyContinue | ForEach-Object {
        $name = $_.Name
        if ($name -notmatch 'hitman|o3de|open.?3d' -and $name -notmatch 'Hitman') { return }
        try {
            $lnk = $shell.CreateShortcut($_.FullName)
            $found += [PSCustomObject]@{
                Name = $name
                Path = $_.FullName
                Target = $lnk.TargetPath
                Arguments = $lnk.Arguments
                WorkingDirectory = $lnk.WorkingDirectory
            }
        } catch {}
    }
}

if ($found.Count -eq 0) {
    Write-Host "No Hitman/O3DE shortcuts found on Desktop or Start Menu." -ForegroundColor Yellow
    exit 0
}

Write-Host ""
Write-Host "Found shortcuts:" -ForegroundColor Cyan
foreach ($item in $found) {
    $status = "UNKNOWN"
    $t = $item.Target + " " + $item.Arguments
    if ($t -match [regex]::Escape($goodEngine)) { $status = "OK (points to NEW engine)" }
    elseif ($t -match 'Hitman3D-new\\hitman3d-engine' -or $t -match 'Hitman3D-new') { $status = "OK (new tree)" }
    elseif ($t -match 'Hitman3D\\engine' -or $t -match 'C:\\Hitman3D\\') { $status = "OLD - safe to delete this shortcut" }
    elseif ($t -match 'O3DEProjectManager|Editor\.exe') { $status = "O3DE app - check Target path below" }

    Write-Host ""
    Write-Host "  $($item.Name)" -ForegroundColor White
    Write-Host "    Status:  $status"
    Write-Host "    Target:  $($item.Target)"
    if ($item.Arguments) { Write-Host "    Args:    $($item.Arguments)" }
    Write-Host "    Folder:  $($item.WorkingDirectory)"
    Write-Host "    File:    $($item.Path)"
}

Write-Host ""
Write-Host "Correct engine folder: $goodEngine" -ForegroundColor Green
