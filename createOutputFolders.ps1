$currentfolder = (Get-Location).Path
$project = "Engine"
$debug_folders = @("Debug", "Editor_Debug")
$release_folders = @("Release", "Development", "Editor_Release", "Editor_Development")
$folders = $debug_folders + $release_folders
$srcfolder = "Target"
$ignorelist= @(".gitignore")
$project_folder = "$($currentfolder)\$($project)"

function SetupFolder {
	param (
		[string]$folder
	)
	
	cmd /c mkdir $project_folder\$folder
	
	$items = Get-ChildItem -Path $project_folder\$srcfolder

	foreach ($item in $items) {
		#ignore things from the ignorelist
		if($item -in $ignorelist) { 
			continue
		}
		
		$symlink = "$($project_folder)\$($folder)\$($item.Name)"
		$srcfile = "$($project_folder)\$($srcfolder)\$($item.Name)"

		# Example: Check if it's a file or a directory
		if ($item.PSIsContainer) {
			Write-Host "Directory Symlink $($srcfile) -> $($symlink)"
			cmd /c mklink /D $symlink $srcfile
		} else {
			Write-Host "File Symlink $($srcfile) -> $($symlink)"
			cmd /c mklink $symlink $srcfile
		}
	}
}

foreach ($folder in $folders) {
	SetupFolder $folder
}