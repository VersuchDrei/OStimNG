
class Settings_MC extends MovieClip
{
	var main:Option_Box_MC;
	var bg:MovieClip;
	var settings:Array = new Array(0);
	var currentlyHighlightedIdx:Number = 0;

	var elementCount = 3;
	public function Settings_MC()
	{
		main = _parent.optionBoxes;
		bg._width = main.menuGutter * 2 + main.optionWidth;
		bg._height = main.menuGutter * 2 + (main.optionHeight * elementCount) + (main.optionGutter * (elementCount - 1));
		bg._x = 0;
		bg._y = 0;
		// constructor code

		GenerateSettings();
	}

	function GenerateSetting(idx:Number):MovieClip
	{
		var mc = this.attachMovie("Option_MC", "s" + idx, idx + 1, {_x:bg._width / 2, _y:0 - (main.menuGutter + ((main.optionGutter + main.optionHeight) * idx) + (main.optionHeight / 2)), _width:main.optionWidth, _height:main.optionHeight});
		mc.DrawBorder('FFFFFF');
		mc.myIdx = idx;
		mc.TextureLoader = main.TextureLoader;
		return mc;
	}

	function GenerateSettings():Void
	{
		for (var i = 0; i < elementCount; i++)
		{
			settings[i] = GenerateSetting(i);
			settings[i].ShowOption();
		}
		settings[0].SetData({Description:"Settings (Coming soon)", ImagePath:"Ostim/icons/OStim/symbols/settings.dds", NodeID:""});
		settings[0].doSelect = function()
		{
			_parent.ShowSettings();
		};
		settings[1].SetData({Description:"Alignment", ImagePath:"Ostim/icons/OStim/symbols/alignment.dds", NodeID:""});
		settings[1].doSelect = function()
		{
			_parent.doShowAlignment();
		};
		settings[2].SetData({Description:"Search", ImagePath:"Ostim/icons/OStim/symbols/search.dds", NodeID:""});
		settings[2].doSelect = function()
		{
			_parent.doShowSearch();
		};
	}

	function EnterMenu()
	{
		Highlight(currentlyHighlightedIdx);
	}
	function LeaveMenu()
	{
		for (var i = 0; i < elementCount; i++)
		{
			settings[i].OnUnHighlight();
		}
	}

	function NavUp():Void
	{
		if (currentlyHighlightedIdx < elementCount - 1)
		{
			settings[currentlyHighlightedIdx].OnUnHighlight();
			currentlyHighlightedIdx++;
			Highlight(currentlyHighlightedIdx);
		}
	}
	function NavDown():Void
	{
		if (currentlyHighlightedIdx > 0)
		{
			settings[currentlyHighlightedIdx].OnUnHighlight();
			currentlyHighlightedIdx--;
			Highlight(currentlyHighlightedIdx);
		}
	}
	
	function SelectSetting(){
		settings[currentlyHighlightedIdx].OnSelect();
	}

	function Highlight(idx:Number):Void
	{
		settings[idx].OnHighlight();
		main.optionDesc.text = settings[idx].Description;
	}

	function ShowSettings()
	{
	}

	function doShowSearch()
	{
	}

	function doShowAlignment()
	{
	}
}