
import com.greensock.*;
class SearchBar extends MovieClip
{
	var bg: MovieClip;
	var divider: MovieClip;
	var textInput: MovieClip;
	
	var Options = new Array(0);
	var CurrentlyHighlightedIdx = -1;
	
	
	var topGutter = 10;
	var bottomGutter = 28;
	var optionGutter = 8;
	var lineHeight = 48;
	var minHeight = lineHeight + optionGutter + bottomGutter;
	
	var maxOptions = 99;
	
	var menuGutter = 22;
	
	public function SearchBar()
	{
		super();
		// constructor code
		bg._width = 355;
		bg._x = bg._width / 2;
		bg._height = minHeight;
		bg._y = 0 - (bg._height/2);
		
		divider._width = bg._width - (menuGutter * 2);
		divider._x = menuGutter;
		
		AssignData(generateTestData(5))//; For Testing
	}
	
	public function AssignData(data : Array)
	{
		if(data.length == 0){
			HideDivider();
		} else {
			ShowDivider();
			var optCount = data.length > maxOptions ? maxOptions : data.length;
			var newHeight = minHeight + topGutter + ((optionGutter + lineHeight) * optCount);
			TweenLite.to(bg,0.5,{
					 //_width:Math.floor(Math.ceil(menuGutterX + (noOfCols * optionWidth) + ((noOfCols - 1) * optionGutterX), minWidth), maxWidth), 
					 _height: newHeight,
					 _y: 0 - (newHeight /2)});
					 
					 
			var inputFormat = textInput.textField.getTextFormat();
			for(var i = 0; i < data.length; i++){
				var field = createTextField("textOpt" + i, this.getNextHighestDepth(),
											menuGutter,
											0 - (minHeight + lineHeight + ((optionGutter + lineHeight) * i)),
											divider._width,
											lineHeight);

				field.text = data[i].label;
				field.textColor = "FFFFFF";
				field.setTextFormat(inputFormat)
			}
		}
	}	
	
	public function HandleSelect(val :String){
		trace(val);
	}
	
	function HideDivider(){		
		divider._alpha = 00;
		divider._visible = false;
	}
	
	function ShowDivider(){
		divider._visible = true;
		divider._alpha = 70;
	}
	
	function generateTestData(count: Number){
		var arr = new Array(count);
		for(var i = 0; i < count; i++){
			arr[i] = {label: i, value: "v" + i};
		}
		return arr;
	}
}