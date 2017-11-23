# PluginColorCursor

Creates the ColorCursor.dll plugin for Rainmeter.

Build in conjunction with the Rainmeter Plugin SDK from [Rainmeter Plugin SDK](https://github.com/rainmeter/rainmeter-plugin-sdk)  
Documentation for the SDK at [Developers Documentation](https://docs.rainmeter.net/developers/#CreatePlugin)

## Options

### Format
`Format=RGB` Returns rrr,ggg,bbb formatted rgb color  
`Format=Red` Returns rrr red color element  
`Format=Green` Returns ggg green color element  
`Format=Blue` Returns bbb blue color element  

### RealTime

`RealTime=0` (default) Plugin will do nothing until called with !CommandMeasure  
`RealTime=1` Plugin will constantly return the color of the pixel under the mouse cursor

If RealTime=1 then other options are valid:  

`ZoomCreate` 0 or 1 to turn off / on returning a zoomed bitmap image of the area around the cursor  
`ZoomToWidth` Number of pixels width for the image to return  
`ZoomToHeight` Number of pixels height for the image to return  
`ZoomFactor` Number of times to "zoom" the area around the cursor  

The plugin will return "ZoomBitmap.bmp" in the current skin folder on each measure update.  
The center of the bitmap will be the current cursor location.  

## Commands 

`[!CommandMeasure MeasureName "GetColor"]`  
When executed in an action, the plugin will return the color of the pixel under the mouse cursor


