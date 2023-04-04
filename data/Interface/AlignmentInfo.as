
class AlignmentInfo extends MovieClip
{

	/* STAGE ELEMENTS */
	public var actorNameData:TextField;
	public var actorSlotData:TextField;
	public var actorGenderData:TextField;
	public var sourceData:TextField;
	public var incrementField:TextField;
	/*Fields*/
	public var xField:InfoField;
	public var yField:InfoField;
	public var zField:InfoField;


	private var selectedField:Number = 0;

	public function AlignmentInfo()
	{
		// constructor code
		super();
		selectField(selectedField);
	}

	public function updateInfo(actorNameValue:String, actorSlotValue:Number, actorGenderValue:String, sourceValue:String, incrementValue:Number, xOffsetValue:Number, yOffsetValue:Number, zOffsetValue:Number)
	{
		actorNameData.text = actorNameValue;
		actorSlotData.text = actorSlotValue.toString();
		actorGenderData.text = actorGenderValue;
		sourceData.text = sourceValue;
		incrementField.text = formatNumber(incrementValue);
		xField.updateText(formatNumber(xOffsetValue));
		yField.updateText(formatNumber(yOffsetValue));
		zField.updateText(formatNumber(zOffsetValue));
	}

	private function updateIncrement(newVal:Number):Void
	{
		incrementField.text = formatNumber(newVal);
	}

	private function selectField(field:Number):Void
	{
		getSelectedField().deselect();
		selectedField = field;
		getSelectedField().select();
	}

	private function updateDoubleField(newVal:Number, up:Boolean):Void
	{
		updateField(formatNumber(newVal, up));
	}
	private function updateField(newVal:String, up:Boolean):Void
	{
		getSelectedField().animateUpdateText(newVal,up);
	}

	private function getSelectedField():InfoField
	{
		switch (selectedField)
		{
			case 0 :
				return xField;
			case 1 :
				return yField;
			case 2 :
				return zField;
		}
	}

	private function formatNumber(val:Number):String
	{
		var init = val.toString();
		var stringParts = init.split(".");
		if (stringParts.length == 1)
		{
			return init + ".000";
		}
		if (stringParts[1].length > 3)
		{
			init = init.substr(0, (stringParts[0].length + 1 + 3));
		}
		for (var i = stringParts[1].length; i < 3; i++)
		{
			init += "0";
		}
		return init;
	}
}