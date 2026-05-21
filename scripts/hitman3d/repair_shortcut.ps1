param(
    [Parameter(Mandatory = $true)][string]$LauncherPath,
    [Parameter(Mandatory = $true)][string]$EngineRoot,
    [switch]$RemoveOneDriveShortcut = $false
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

$iconPath = Join-Path $engineRoot 'Code\Editor\res\lyeditor_small.ico'
if (-not (Test-Path $iconPath)) {
    $iconPath = Join-Path $engineRoot 'Code\Editor\res\o3de_editor.ico'
}
if (-not (Test-Path $iconPath)) {
    $iconPath = "$launcherPath,0"
}

function Set-StudioShortcut {
    param([string]$Path)
    $dir = Split-Path $Path -Parent
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }
    $sc = $shell.CreateShortcut($Path)
    $sc.TargetPath = $launcherPath
    $sc.WorkingDirectory = $engineRoot
    $sc.Description = 'Hitman 3d by jss - Jeffrsin Str33t Studios'
    $sc.IconLocation = $iconPath
    $sc.Save()
}

Write-Host ""
Write-Host "Launcher: $launcherPath" -ForegroundColor Cyan
Write-Host ""

# Create on BOTH desktops first (Windows often redirects Desktop to OneDrive)
$targets = @($localShortcut)
if ((Test-Path $oneDriveDesktop) -and ($oneDriveDesktop -ne $localDesktop)) {
    $targets += $oneDriveShortcut
}

foreach ($t in $targets) {
    Set-StudioShortcut -Path $t
    Write-Host "Created/updated shortcut:" -ForegroundColor Green
    Write-Host "  $t"
}

# Only remove OneDrive copy when we are putting shortcut on local Desktop only
if ($RemoveOneDriveShortcut -and (Test-Path $oneDriveShortcut) -and (Test-Path $localShortcut)) {
    $localResolved = (Resolve-Path $localShortcut).Path
    $odResolved = (Resolve-Path $oneDriveShortcut).Path
    if ($localResolved -ne $odResolved) {
        Remove-Item -LiteralPath $oneDriveShortcut -Force
        Write-Host ""
        Write-Host "Removed duplicate OneDrive shortcut:" -ForegroundColor Yellow
        Write-Host "  $oneDriveShortcut"
    }
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
