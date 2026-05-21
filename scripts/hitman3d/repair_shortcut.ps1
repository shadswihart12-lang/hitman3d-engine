param(
    [Parameter(Mandatory = $true)][string]$LauncherPath,
    [Parameter(Mandatory = $true)][string]$EngineRoot,
    [switch]$RemoveOneDriveShortcut = $true
)

$launcherPath = (Resolve-Path $LauncherPath).Path
$engineRoot = (Resolve-Path $EngineRoot).Path

$shell = New-Object -ComObject WScript.Shell
$shortcutName = 'Hitman 3d by jss.lnk'

# Prefer a LOCAL desktop folder (not OneDrive sync)
$localDesktop = Join-Path $env:USERPROFILE 'Desktop'
if (-not (Test-Path $localDesktop)) {
    New-Item -ItemType Directory -Path $localDesktop -Force | Out-Null
}

$oneDriveDesktop = Join-Path $env:USERPROFILE 'OneDrive\Desktop'
$oneDriveShortcut = Join-Path $oneDriveDesktop $shortcutName
$localShortcut = Join-Path $localDesktop $shortcutName

function Set-StudioShortcut {
    param([string]$Path)
    $sc = $shell.CreateShortcut($Path)
    $sc.TargetPath = $launcherPath
    $sc.WorkingDirectory = $engineRoot
    $sc.Description = 'Hitman 3d by jss - Jeffrsin Str33t Studios'
    $sc.IconLocation = "$launcherPath,0"
    $sc.Save()
}

Write-Host ""
Write-Host "Launcher: $launcherPath" -ForegroundColor Cyan
Write-Host ""

# Create / update LOCAL desktop shortcut (primary)
Set-StudioShortcut -Path $localShortcut
Write-Host "Created/updated LOCAL desktop shortcut:" -ForegroundColor Green
Write-Host "  $localShortcut"

# Remove OneDrive desktop copy so you only use one shortcut
if ($RemoveOneDriveShortcut -and (Test-Path $oneDriveShortcut)) {
    Remove-Item -LiteralPath $oneDriveShortcut -Force
    Write-Host ""
    Write-Host "Removed OneDrive shortcut (no longer used):" -ForegroundColor Yellow
    Write-Host "  $oneDriveShortcut"
}

# Remove duplicate Studio name on OneDrive if present
$odStudio = Join-Path $oneDriveDesktop 'Hitman 3d Studio.lnk'
if ($RemoveOneDriveShortcut -and (Test-Path $odStudio)) {
    Remove-Item -LiteralPath $odStudio -Force
    Write-Host "Removed: $odStudio"
}

$read = $shell.CreateShortcut($localShortcut)
Write-Host ""
Write-Host "Use this shortcut from now on:" -ForegroundColor Cyan
Write-Host "  $($read.FullName)"
Write-Host "  Target: $($read.TargetPath)"
Write-Host "  Status: OK" -ForegroundColor Green

Write-Host ""
Write-Host "Tip: Keep engine and projects OUT of OneDrive folders." -ForegroundColor DarkGray
Write-Host "  Engine:  $engineRoot"
Write-Host "  Project: $(Join-Path (Split-Path $engineRoot -Parent) 'projects\Hitman3DStudio')"
