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
	
	
	var optionGutterX = 14;
	var optionGutterY = 14;	
	var menuGutterX = 48;
	var menuGutterY = 98;	
	var optionWidth = 86;
	var optionHeight = 86;
	
	var minWidth = menuGutterX + 150;
	var maxWidth = menuGutterX + 3*optionWidth + 2*optionGutterX;
	
	var TextureLoader; 

	public function Option_Box_MC()
	{
		super();
		CurrentlyHighlightedIdx = -1;

		TextureLoader = new MovieClipLoader();
		TextureLoader.addListener(this);
	}

	public function HandleKeyboardInput(e:Number)
	{
		// 0 = up, 1 = down, 2 = left, 3 = right, 4 = select;

		if (maxOptionIdx == -1)
		{
			return;
		}
		// Sanitize starting idx 
		if (CurrentlyHighlightedIdx < 0)
		{
			CurrentlyHighlightedIdx = 0;
			Highlight();	
		}
		else
		{
			Options[CurrentlyHighlightedIdx].OnUnHighlight();

			var col = getCol(CurrentlyHighlightedIdx);
			var row = Math.floor(CurrentlyHighlightedIdx / 3);

			var colMin = col;
			var colMax = maxOptionIdx - (((maxOptionIdx % 3) - col) % 3);

			var rowMin = 3 * row;
			var rowMax = Math.min(maxOptionIdx, (3 * row) + 2);

			switch (e)
			{
				case 0 :
					CurrentlyHighlightedIdx = RollOver(CurrentlyHighlightedIdx + 3, colMin, colMax);
					break;
				case 1 :
					CurrentlyHighlightedIdx = RollOver(CurrentlyHighlightedIdx - 3, colMin, colMax);
					break;
				case 2 :
					CurrentlyHighlightedIdx = RollOver(CurrentlyHighlightedIdx - 1, rowMin, rowMax);
					break;
				case 3 :
					CurrentlyHighlightedIdx = RollOver(CurrentlyHighlightedIdx + 1, rowMin, rowMax);
					break;
				case 4 :
					Options[CurrentlyHighlightedIdx].OnSelect();
					break;
			}
		}
		Highlight();
	}
	
	function Highlight(idx: Number){
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
		for(var j = 0; j < Options.length; j++){
			Options[j].HideOption();
		}
		
		maxOptionIdx = Edges.length - 1;
		var maxOptionRow = Math.floor(maxOptionIdx / 3);
		var noOfCols = maxOptionIdx >= 3 ? 3 : maxOptionIdx + 1;
		TweenLite.to(bg, 0.5,
						{
							_width: Math.floor(Math.ceil(menuGutterX + (noOfCols * optionWidth) + ((noOfCols - 1) * optionGutterX), minWidth), maxWidth),
							_height: menuGutterY + ((maxOptionRow + 1) * optionHeight) + (maxOptionRow * optionGutterY)
						});

		if(Edges.length == 0){
			TweenLite.to(this, 0.2, {_alpha:0});
			return;
		} else{
			TweenLite.to(this, 0.5, {_alpha:100});
		}
		for (var i = 0; i < Edges.length; i++)
		{
			if(Options.length <= i){
				Options.push(GenerateOption(i, getCol(i), getRow(i)));
			}
			Options[i].SetData(Edges[i]);
			Options[i].ShowOption();
		}
		CurrentlyHighlightedIdx = 0;
		Highlight();
	}
	
	function getCol(idx) : Number 
	{
		return idx % 3;
	}
	
	function getRow(idx) : Number 	
	{
		return Math.floor(idx / 3);
	}
	
	public function GenerateOption(idx, colIdx, rowIdx): MovieClip
	{
		var mc = this.attachMovie("Option_MC", "o"+idx, idx+1,{
								  _x: (menuGutterX / 2) + (colIdx * (optionWidth + optionGutterX)) + (optionWidth / 2),
								  _y: 0-((67) + (rowIdx * (optionHeight + optionGutterY)) + (optionHeight / 2)), 
								  _width: optionWidth, 
								  _height: optionHeight
								  });
								  
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
		if (undefined != this.TextureLoader ) 
		{
			if (undefined != aTargetClip) 
			{
				aTargetClip._width = 82;
				aTargetClip._height = 82;
			}
		}
	}
}