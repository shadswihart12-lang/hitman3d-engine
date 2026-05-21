param(
    [Parameter(Mandatory = $true)][string]$LauncherPath,
    [Parameter(Mandatory = $true)][string]$EngineRoot
)

$launcherPath = (Resolve-Path $LauncherPath).Path
$engineRoot = (Resolve-Path $EngineRoot).Path

$desktopCandidates = @(
    [Environment]::GetFolderPath('Desktop'),
    (Join-Path $env:USERPROFILE 'OneDrive\Desktop'),
    (Join-Path $env:USERPROFILE 'OneDrive - Personal\Desktop')
) | Where-Object { $_ -and (Test-Path $_) } | Select-Object -Unique

$shortcutNames = @(
    'Hitman 3d by jss.lnk',
    'Hitman 3d Studio.lnk'
)

$shell = New-Object -ComObject WScript.Shell

Write-Host ""
Write-Host "Repairing shortcuts to point at:" -ForegroundColor Cyan
Write-Host "  $launcherPath"
Write-Host ""

foreach ($desktop in $desktopCandidates) {
    foreach ($name in $shortcutNames) {
        $lnkPath = Join-Path $desktop $name
        $sc = $shell.CreateShortcut($lnkPath)
        $sc.TargetPath = $launcherPath
        $sc.WorkingDirectory = $engineRoot
        $sc.Description = 'Hitman 3d by jss - Jeffrsin Str33t Studios'
        $sc.IconLocation = "$launcherPath,0"
        $sc.Save()
        Write-Host "Updated: $lnkPath" -ForegroundColor Green
    }
}

# Report any existing "Hitman 3d by jss" before/after style audit
$yourShortcut = Join-Path $env:USERPROFILE 'OneDrive\Desktop\Hitman 3d by jss.lnk'
if (-not (Test-Path $yourShortcut)) {
    $yourShortcut = Join-Path ([Environment]::GetFolderPath('Desktop')) 'Hitman 3d by jss.lnk'
}
if (Test-Path $yourShortcut) {
    $read = $shell.CreateShortcut($yourShortcut)
    Write-Host ""
    Write-Host "Your shortcut now:" -ForegroundColor Cyan
    Write-Host "  File:   $yourShortcut"
    Write-Host "  Target: $($read.TargetPath)"
    Write-Host "  Start:  $($read.WorkingDirectory)"
    if ($read.TargetPath -eq $launcherPath) {
        Write-Host "  Status: OK" -ForegroundColor Green
    }
}
