// original script by osmosis-wrench 2022
import skse;
import gfx.managers.FocusHandler;
import Mouse;
import com.greensock.*;

class Option_Box_MC extends MovieClip
{
	var o0 : MovieClip;
	var o1 : MovieClip;
	var o2 : MovieClip;
	var o3 : MovieClip;
	var o4 : MovieClip;
	var o5 : MovieClip;
	var o6 : MovieClip;
	var o7 : MovieClip;
	var o8 : MovieClip;
	var o9 : MovieClip;
	var o10 : MovieClip;
	var o11 : MovieClip; 
	
	var bg : MovieClip;
	var optionDesc: TextField;
	
	var Options: Array = new Array(12);
	var CurrentlyHighlightedIdx: Number;
	var maxOptionIdx :Number = -1;
	
	public function Option_Box_MC()
	{
		super();
		CurrentlyHighlightedIdx = -1;
		Options = [o0, o1, o2, o3, o4, o5, o6, o7, o8 ,o9 , o10, o11];
		
		for (var i in Options){
			Options[i].myIdx = Number(i);
			//Mouse func
			/*Options[i].onRollOver = function(){
				this.OnHighlight();
			};
			Options[i].onRollOut = function(){
				this.OnUnHighlight();
			};
			Options[i].onMouseDown = function(){
				//trace(_parent);
				if (Mouse.getTopMostEntity()._parent == this){
					this.OnSelect();
				}
			};*/
		}
	}
	
	public function HandleKeyboardInput(e:Number)
	{
		// 0 = up, 1 = down, 2 = left, 3 = right, 4 = select;
		
		if(maxOptionIdx == -1){return;}
		
		// Sanitize starting idx
		trace("Handle "+CurrentlyHighlightedIdx);
		if (CurrentlyHighlightedIdx < 0) {
			CurrentlyHighlightedIdx = 0;
			Options[CurrentlyHighlightedIdx].OnHighlight();
		} else {
			Options[CurrentlyHighlightedIdx].OnUnHighlight();
			trace("inner "+CurrentlyHighlightedIdx);
			
			var col = CurrentlyHighlightedIdx % 3;
			var row = Math.floor(CurrentlyHighlightedIdx / 3);
			
			var colMin = col;
			var colMax = maxOptionIdx - (((maxOptionIdx % 3) - col ) %3 );
			
			var rowMin = 3 * row;
			var rowMax = Math.min(maxOptionIdx, (3 * row) + 2);
			
			switch(e){
				case 0:
					trace("up");
					CurrentlyHighlightedIdx = RollOver(CurrentlyHighlightedIdx + 3, colMin, colMax);
					break;
				case 1:
					trace("down");
					CurrentlyHighlightedIdx = RollOver(CurrentlyHighlightedIdx - 3, colMin, colMax);
					break;
				case 2:
					trace("left");
					CurrentlyHighlightedIdx = RollOver(CurrentlyHighlightedIdx - 1, rowMin, rowMax);
					break;
				case 3:
					trace("right");
					CurrentlyHighlightedIdx = RollOver(CurrentlyHighlightedIdx + 1, rowMin, rowMax);
					break;
				case 4:
					trace("select");
					Options[CurrentlyHighlightedIdx].OnSelect();
					break;
			}
		}
		
		Options[CurrentlyHighlightedIdx].OnHighlight();
		optionDesc.text = Options[CurrentlyHighlightedIdx].Description;
		trace("Out "+CurrentlyHighlightedIdx);
		trace("---");
	}
	
	
	function RollOver(val: Number,min :Number, max: Number) : Number{
		if(val < min){
			return max;
		} else if (val > max){
			return min;
		} else{
			return val;
		}
		
	}
	
	
	public function SendTransitionRequest(NodeID:Number)
	{
		log("UI_TransitionRequest {}, {"+NodeID+"}");
		skse.SendModEvent("UI_TransitionRequest", null, NodeID);
	}
	
	public function SendSpeedRequest(SpeedIdx:Number)
	{
		log("UI_SpeedRequest {}, {"+SpeedIdx+"}");
		skse.SendModEvent("UI_SpeedRequest", null, SpeedIdx);
	}
	
	public function AssignData(Edges:Array)
	{
		var i;
		for(i in Edges){
			Options[i].SetData(Edges[i]);
		}
		maxOptionIdx = Edges.length-1;
		var maxOptionRow = Math.floor(maxOptionIdx / 3)
		var noOfCols = maxOptionIdx > 3 ? 3 : maxOptionIdx;
		TweenLite.to(bg, 0.5, {
					 _width: (48 + (noOfCols * 86) + ((noOfCols - 1) * 14)), 
					 _height: (48 + ((maxOptionRow + 1) * 86) + maxOptionRow * 14) + 50
					 });
	}
	
	function log(str:String)
	{
		_root.test1.text = str;
		trace(str);
	}
}