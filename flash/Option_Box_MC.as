// original script by osmosis-wrench 2022
import skse;
import gfx.managers.FocusHandler;
import Mouse;
import com.greensock.*;
class Option_Box_MC extends MovieClip
{
	var bg:MovieClip;
	var optionDesc:TextField;

	var Options:Array = new Array(0);
	var CurrentlyHighlightedIdx:Number;
	var maxOptionIdx:Number = -1;


	var optionGutter;
	var optionGutterX;
	var optionGutterY;

	var menuGutter;
	var menuGutterX;

	var menuGutterYText;
	var menuGutterYBase;
	var menuGutterYTop;
	var menuGutterY;

	var optionWidth;
	var optionHeight;

	var minWidth;
	var maxWidth;

	var TextureLoader;

	var speedUp:MovieClip;
	var speedDown:MovieClip;
	var speedDesc:TextField;


	public function Option_Box_MC()
	{
		super();
		CurrentlyHighlightedIdx = -1;

		TextureLoader = new MovieClipLoader();
		TextureLoader.addListener(this);

		optionGutter = 14;
		optionGutterX = optionGutter;
		optionGutterY = optionGutter;

		menuGutter = 28;
		menuGutterX = menuGutter * 2;

		menuGutterYText = 86;
		menuGutterYBase = menuGutterYText + optionGutter;
		menuGutterYTop = menuGutter;
		menuGutterY = menuGutterYBase + menuGutterYTop;

		optionWidth = 86;
		optionHeight = 86;

		minWidth = menuGutterX + 150;
		maxWidth = menuGutterX + (3 * optionWidth) + (2 * optionGutterX);

		bg._width = maxWidth;
		bg._x = bg._width / 2;
      	this.speedUp.stop();
      	this.speedDown.stop();

		//AssignData(generateTestData())// For testing in flash
	}

	public function HandleKeyboardInput(e:Number)
	{
		// 0 = up, 1 = down, 2 = left, 3 = right, 4 = select;

		if (maxOptionIdx == -1)
		{
			return;
		}
		var col = getCol(CurrentlyHighlightedIdx);
		var row = Math.floor(CurrentlyHighlightedIdx / 3);

		var colMin = col;
		var colMax = maxOptionIdx - ((3 - (((col - (maxOptionIdx % 3)) + 3) % 3)) % 3);

		var rowMin = 3 * row;
		var rowMax = Math.min(maxOptionIdx, (3 * row) + 2);

		switch (e)
		{
			case 0 :
				Highlight(RollOver(CurrentlyHighlightedIdx + 3, colMin, colMax));
				break;
			case 1 :
				Highlight(RollOver(CurrentlyHighlightedIdx - 3, colMin, colMax));
				break;
			case 2 :
				Highlight(RollOver(CurrentlyHighlightedIdx - 1, rowMin, rowMax));
				break;
			case 3 :
				Highlight(RollOver(CurrentlyHighlightedIdx + 1, rowMin, rowMax));
				break;
			case 4 :
				Options[CurrentlyHighlightedIdx].OnSelect();
				break;
		}

	}

	function Highlight(idx:Number)
	{
		if (CurrentlyHighlightedIdx != -1)
		{
			Options[CurrentlyHighlightedIdx].OnUnHighlight();
		}
		CurrentlyHighlightedIdx = idx;
		Options[CurrentlyHighlightedIdx].OnHighlight();
		optionDesc.text = Options[CurrentlyHighlightedIdx].Description;
	}


	function RollOver(val:Number, min:Number, max:Number):Number
	{
		if (val < min)
		{
			return max;
		}
		else if (val > max)
		{
			return min;
		}
		else
		{
			return val;
		}
	}


	public function SendTransitionRequest(NodeID:String)
	{
		log("UI_TransitionRequest {}, {" + NodeID + "}");
		doSendTransitionRequest(NodeID);
	}

	public function doSendTransitionRequest(sceneId:String)
	{
	}

	public function SendSpeedRequest(SpeedIdx:Number)
	{
		log("UI_SpeedRequest {}, {" + SpeedIdx + "}");
		skse.SendModEvent("UI_SpeedRequest",null,SpeedIdx);
	}

	public function AssignData(Edges:Array)
	{
		for (var j = 0; j < Options.length; j++)
		{
			Options[j].HideOption();
		}

		maxOptionIdx = Edges.length - 1;
		var maxOptionRow = Math.floor(maxOptionIdx / 3);
		var noOfCols = maxOptionIdx >= 3 ? 3 : maxOptionIdx + 1;
		var newHeight = menuGutterY + ((maxOptionRow + 1) * optionHeight) + (maxOptionRow * optionGutterY);
		//_width:Math.floor(Math.ceil(menuGutterX + (noOfCols * optionWidth) + ((noOfCols - 1) * optionGutterX), minWidth), maxWidth), 
		TweenLite.to(bg,0.5,{_height:newHeight, _y:0 - (newHeight / 2)});

		if (Edges.length == 0)
		{
			TweenLite.to(this,0.2,{_alpha:0});
			return;
		}
		else
		{
			TweenLite.to(this,0.5,{_alpha:100});
		}
		for (var i = 0; i < Edges.length; i++)
		{
			if (Options.length <= i)
			{
				Options.push(GenerateOption(i, getCol(i), getRow(i)));
			}
			Options[i].SetData(Edges[i]);
			Options[i].ShowOption();
		}
		CurrentlyHighlightedIdx = -1;
		Highlight(0);
	}

	function getCol(idx):Number
	{
		return idx % 3;
	}

	function getRow(idx):Number
	{
		return Math.floor(idx / 3);
	}

	public function GenerateOption(idx, colIdx, rowIdx):MovieClip
	{
		var mc = this.attachMovie("Option_MC", "o" + idx, idx + 1, {_x:(menuGutterX / 2) + (colIdx * (optionWidth + optionGutterX)) + (optionWidth / 2), _y:0 - ((menuGutterYBase) + (rowIdx * (optionHeight + optionGutterY)) + (optionHeight / 2)), _width:optionWidth, _height:optionHeight});

		mc.myIdx = idx;
		mc.TextureLoader = this.TextureLoader;
		return mc;
	}

	function log(str:String)
	{
		trace(str);
	}

	function onLoadInit(aTargetClip)
	{
		if (undefined != this.TextureLoader)
		{
			if (undefined != aTargetClip)
			{
				aTargetClip._width = 82;
				aTargetClip._height = 82;
			}
		}
	}

	function generateTestData()
	{
		var arr = new Array(15);
		for (var i = 0; i < arr.length; i++)
		{
			var obj = {};
			obj.Border = "FFFFFF";
			obj.ImagePath = "test" + i + ".dds";
			obj.Description = "Standing, holding or some other description thats long";
			arr[i] = obj;
		}
		return arr;
	}

	function ShowSpeed(speedVal:String, showUp:Boolean, showDown:Boolean)
	{
		trace("speed " + speedVal);

		speedUp._visible = true;
		speedUp.Show(showUp);

		speedDown._visible = true;
		speedDown.Show(showDown);

		speedDesc._visible = true;
		speedDesc.text = speedVal;
	}

	function SpeedUp()
	{
		speedUp.play();
	}
	function SpeedDown()
	{
		speedDown.play();
	}

	function HideSpeed()
	{
		speedUp._visible = false;
		speedDown._visible = false;
		speedDesc._visible = false;
	}
}