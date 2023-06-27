// by osmosis-wrench 2022

class Info_Box_MC extends MovieClip
{
	var Animation_Name_Text: TextField;
	var Animation_Speed_Text: TextField;
	var Animation_Animator_Text: TextField;
	var CurrentNodeID: Number;
	
	function Info_Box_MC()
	{
		super();
	}
	
	function AssignData(Current:Object)
	{
		CurrentNodeID = Current["NodeID"];
		Animation_Name_Text.text = Current["Title"];
		Animation_Speed_Text.text = "Speed: " + Current["CurrentSpeed"] + "/" + Current["MaxSpeed"];
		Animation_Animator_Text.text = Current["Animator"];
	}
}