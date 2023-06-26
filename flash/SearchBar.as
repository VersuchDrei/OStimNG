
import com.greensock.*;
import gfx.ui.InputDetails;
import Shared.GlobalFunc;
import gfx.ui.NavigationCode;
import gfx.managers.FocusHandler;

class SearchBar extends MovieClip
{
	var bg:MovieClip;
	var divider:MovieClip;

	var Options = new Array(0);
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
	var inputtingText:Boolean = false;
	var fakeButton;

	public function SearchBar()
	{
		super();
		// constructor code
		bg._width = 355;
		bg._x = bg._width / 2;
		bg._height = minHeight;
		bg._y = 0 - (bg._height / 2);

		divider._width = bg._width - (menuGutter * 2);
		divider._x = menuGutter;


		//AssignData(generateTestData(5));//; For Testing
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
					return true;
				}
				else
				{
					SelectOption();
					return true;
				}
			}
			else if (details.navEquivalent == NavigationCode.TAB || details.navEquivalent == NavigationCode.ESCAPE)
			{
				ClearAndHide();
				return true;
			}
			else if (details.navEquivalent == NavigationCode.UP && !inputtingText)
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
				return true;
			}
			else if (details.navEquivalent == NavigationCode.DOWN && !inputtingText)
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
				return true;
			}

			var nextClip = pathToFocus.shift();
			if (nextClip.handleInput(details, pathToFocus))
			{
				return true;
			}
		}

		return false;
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
		ClearInput();
		AssignData([]);
		doHideMenuRequest();
	}
	function ClearInput()
	{
		textInput.text = "";
	}
	function DisableTextInput(){		
		inputtingText = false;
		Selection.setFocus(fakeButton);
	}
	function EnableTextInput()
	{
		Selection.setFocus(textInput.textField);
		inputtingText = true;
	}

	public function AssignData(data:Array)
	{
		for (var k = 0; k < fields.length; k++)
		{
			fields[k]._visible = false;
		}

		if (data.length == 0)
		{
			HideDivider();
			var newHeight = minHeight;
			TweenLite.to(bg,0.5,{_height:newHeight, _y:0 - (newHeight / 2)});

			CurrentlySelectedIdx = -1;
			EnableTextInput();
		}
		else
		{
			ShowDivider();
			var optCount = data.length > maxOptions ? maxOptions : data.length;
			var newHeight = minHeight + topGutter + ((optionGutter + lineHeight) * optCount);
			TweenLite.to(bg,0.5,{_height:newHeight, _y:0 - (newHeight / 2)});

			for (var i = 0; i < optCount; i++)
			{
				if (fields.length < i + 1)
				{
					fields.push(this.attachMovie("SearchOption", "o" + i, i + 1, {_x:menuGutter, _y:0 - (minHeight + lineHeight + ((optionGutter + lineHeight) * i)), _height:lineHeight}));

				}
				fields[i]._visible = true;

				fields[i].optionVal.text = data[i].label;
				fields[i].idx = i;
				fields[i].onRollOver = function()
				{
					_parent.SetIdx(this);
					_parent.UpdateHighlight();
				};
				fields[i].onRollOut = function()
				{
					this.OnUnHighlight();
				};
				fields[i].onMouseDown = function()
				{
					if (Mouse.getTopMostEntity()._parent == this)
					{
						_parent.SelectOption();
					}
				};
			}
			CurrentlySelectedIdx = 0;
			DisableTextInput();
		}
		UpdateHighlight();
	}

	public function SetIdx(field)
	{
		CurrentlySelectedIdx = field.idx;
	}

	public function SetIsOpen(isOpen:Boolean)
	{
		trace(Selection.getFocus());
		this._isOpen = isOpen;
		if (_isOpen)
		{
			Selection.setFocus(textInput.textField);
			Selection.setSelection(0,0);
			inputtingText = true;
		}
	}

	public function SelectOption()
	{
		var val = fields[CurrentlySelectedIdx].optionVal.text;
		if (val != undefined)
		{
			doSelectOption(val);
			ClearAndHide();
		}
	}
	public function doSelectOption(val:String)
	{

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
			arr[i] = {label:i, value:"v" + i};
		}
		return arr;
	}

	function doHideMenuRequest()
	{

	}

	function Search()
	{
		doSearch(textInput.text);
	}

	function doSearch(val:String)
	{

	}

}