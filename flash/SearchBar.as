
import com.greensock.*;
import gfx.ui.InputDetails;
import Shared.GlobalFunc;
import gfx.ui.NavigationCode;
import gfx.managers.FocusHandler;

class SearchBar extends MovieClip
{
	var bg:MovieClip;
	var divider:MovieClip;

	var CurrentlySelectedIdx = -1;
	var textInput:MovieClip;

	var topGutter = 10;
	var bottomGutter = 28;
	var optionGutter = 8;
	var lineHeight = 48;
	var minHeight = lineHeight + optionGutter + bottomGutter;

	var maxOptions = 10;

	var menuGutter = 22;

	var _isOpen = false;

	var fields = new Array();
	var PrevFocus:MovieClip;
	var _inputtingText:Boolean = false;
	function set inputtingText(val:Boolean)
	{
		_inputtingText = val;
		doSetInputtingText(val);
	}
	function get inputtingText()
	{
		return _inputtingText;
	}
	var fakeButton;

	var widthTweenTime = 0.25;
	var heightTweenTime = 0.25;

	public function SearchBar()
	{
		super();
		// constructor code
		bg._width = 0;
		bg._height = 0;
		UpdateSize(minHeight,355);

		// For Testing
		//AssignData(generateTestData(5));
	}
	
	//Update size based on the size of the contents. Width outer gutters
	public function UpdateSize(newHeight:Number, newWidth:Number)
	{
		TweenLite.to(bg,widthTweenTime,{_width:newWidth + (menuGutter * 2), _x:(newWidth + (menuGutter * 2)) / 2});
		TweenLite.to(divider,0.5,{_width:newWidth, _x:menuGutter});
		TweenLite.to(bg,heightTweenTime,{delay:widthTweenTime, _height:newHeight, _y:0 - (newHeight / 2)});
	}

	public function HandleKeyboardInput(input:Number)
	{
		// 0 = up, 1 = down, 2 = left, 3 = right, 4 = select, 5 = escape;
		switch (input)
		{
			case 0 :
				{
					if (!inputtingText)
					{
						if (CurrentlySelectedIdx + 1 > fields.length - 1)
						{
							CurrentlySelectedIdx = fields.length - 1;
							UpdateHighlight();
						}
						else
						{
							CurrentlySelectedIdx++;
							UpdateHighlight();
						}
					}
					return true;
				};
				break;
			case 1 :
				{
					if (!inputtingText)
					{
						if (CurrentlySelectedIdx - 1 < 0)
						{
							CurrentlySelectedIdx = 0;
							UpdateHighlight();
						}
						else
						{
							CurrentlySelectedIdx--;
							UpdateHighlight();
						}
					}
					return true;
				};
				break;
			case 4 :
				{
					if (inputtingText)
					{

						return true;
					}
					else
					{
						SelectOption();
						return true;
					}
				};
				break;
			case 5 :
				{
					ClearAndHide();
					return true;
				};
				break;
			default :
				return;
		}
	}
	public function handleInput(details:InputDetails, pathToFocus:Array):Boolean
	{
		if (GlobalFunc.IsKeyPressed(details))
		{
			if (details.navEquivalent == NavigationCode.ENTER && details.code != 32)
			{
				if (inputtingText)
				{
					Search();
					ClearInput();
					DisableTextInput();
					return true;
				}
			}
			else if (details.navEquivalent == NavigationCode.TAB || details.navEquivalent == NavigationCode.ESCAPE)
			{
				ClearAndHide();
				return true;
			}
		}
	}

	function onMouseDown()
	{
		if (Mouse.getTopMostEntity() == textInput.textField)
		{
			EnableTextInput();
		}
	}

	function UpdateHighlight()
	{
		for (var i = 0; i < fields.length; i++)
		{
			fields[i].OnUnHighlight();
		}
		if (CurrentlySelectedIdx != -1)
		{
			fields[CurrentlySelectedIdx].OnHighlight();
		}

	}

	function ClearAndHide()
	{
		doHideMenuRequest();
	}
	function ClearInput()
	{
		textInput.text = "";
	}
	function DisableTextInput()
	{
		inputtingText = false;
		textInput.editable = false;
		gfx.managers.FocusHandler.instance.setFocus(fakeButton,0);
		Selection.setFocus(fakeButton);
	}
	function EnableTextInput()
	{
		textInput.editable = true;
		gfx.managers.FocusHandler.instance.setFocus(textInput.textField,0);
		Selection.setFocus(textInput.textField);
		inputtingText = true;
	}

	public function AssignData(data:Array)
	{
		for (var k = 0; k < fields.length; k++)
		{
			fields[k]._visible = false;
			fields[k].optionVal.text = "";
		}

		if (data.length == 0)
		{
			HideDivider();
			var newHeight = minHeight;
			TweenLite.to(bg,0.5,{_height:newHeight, _y:0 - (newHeight / 2), _width:355, _x:355 / 2});

			CurrentlySelectedIdx = -1;
			EnableTextInput();
		}
		else
		{
			ShowDivider();
			var optCount = data.length > maxOptions ? maxOptions : data.length;

			for (var i = 0; i < optCount; i++)
			{
				if (fields.length < i + 1)
				{
					fields.push(this.attachMovie("SearchOption", "o" + i, i + 1, {_x:menuGutter, _y:0 - (minHeight + lineHeight + ((optionGutter + lineHeight) * i)), _height:lineHeight}));
					fields[i]._visible = false;
				}
				fields[i].init(i,data[i]);

			}
			var maxWidth = 340;
			for (var j = 0; j < optCount; j++)
			{
				if (fields[j].optionVal.textWidth > maxWidth)
				{
					maxWidth = fields[j].optionVal.textWidth;
				}
			}

			var newHeight = minHeight + topGutter + ((optionGutter + lineHeight) * optCount);
			UpdateSize(newHeight,maxWidth);

			for (var i2 = 0; i2 < optCount; i2++)
			{
				TweenLite.delayedCall((widthTweenTime + (heightTweenTime * ((i2 / optCount)))),makeVisible,[fields[i2]]);
			}

			CurrentlySelectedIdx = 0;
			DisableTextInput();
		}
		UpdateHighlight();
	}
	function makeVisible(field)
	{
		field._visible = true;
	}
	public function SetIdx(field)
	{
		CurrentlySelectedIdx = field.idx;
	}

	public function SetIsOpen(isOpen:Boolean)
	{
		this._isOpen = isOpen;
		if (_isOpen)
		{
			EnableTextInput();
			ClearInput();
		}
		else
		{
			ClearInput();
			AssignData([]);
			DisableTextInput();
		}
	}

	public function SelectOption()
	{
		var val = fields[CurrentlySelectedIdx].sceneid;
		if (val != undefined)
		{
			doSelectOption(val);
			ClearAndHide();
		}
	}


	function HideDivider()
	{
		divider._alpha = 00;
		divider._visible = false;
	}

	function ShowDivider()
	{
		divider._visible = true;
		divider._alpha = 70;
	}

	function generateTestData(count:Number)
	{
		_isOpen = true;
		var arr = new Array(count);
		for (var i = 0; i < count; i++)
		{
			arr[i] = {label:"AAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBB" + i, value:"v" + i};
		}
		return arr;
	}

	function Search()
	{
		doSearch(textInput.text);
	}

	public function doSelectOption(val:String)
	{

	}

	function doHideMenuRequest()
	{

	}

	function doSearch(val:String)
	{

	}

	function doSetInputtingText(inputting:Boolean)
	{

	}

}