class SearchBar extends MovieClip
{
	var bg: MovieClip;
	var divider: MovieClip;
	
	var Options = new Array(0);
	var CurrentlyHighlightedIdx = -1;
	
	var minHeight = 50;
	
	var optionGutter = 5;
	var lineHeight = 25;
	var maxOptions = 7;
	
	public function SearchBar()
	{
		super();
		// constructor code
		bg._width = 355;
		bg._height = 50;
	}
	
	public function AssignData(data : Array)
	{
		if(data.length == 0){
			HideDivider();
		} else {
			ShowDivider();
			var optCount = data.length > maxOptions ? maxOptions : data.length;
			bg._height = minHeight + ((optionGutter + lineHeight) * optCount);
		}
	}
	
	function HideDivider(){		
		divider._alpha = 70;
		divider._visible = true;
	}
	
	function ShowDivider(){
		divider._visible = false;
		divider._alpha = 0;
	}
}