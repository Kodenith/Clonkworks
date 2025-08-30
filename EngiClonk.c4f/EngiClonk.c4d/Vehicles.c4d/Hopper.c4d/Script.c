/*-- Neues Script --*/

#strict 3
#include LORY

local ConnectedTo;

func Deposit(){
	if(ConnectedTo){
		ConnectedTo->GrabContents(this());
		DrawParticleWire("HopperPipe",GetX(),GetY(), ConnectedTo, RGBa(100,100,100,0),RGBa(150,150,150,0));
		if(ObjectDistance(this(), ConnectedTo) > 1200){
			Sound("Click");
			Message("$HopperDisconnectDistance$",this());
			ConnectedTo = false;
			return(0);
		}
	}
}

func ControlDigDouble(pByObject){
	[$ConnectH$|Image=SPIP]
	if(ConnectedTo){
		Sound("Click");
		Message("$HopperDisconnect$",this());
		ConnectedTo = false;
	}else{
		if(ConnectedTo = FindObject2(Find_Distance(30), Find_OCF(OCF_Container), Find_Not(Find_OCF(OCF_Alive), Find_OCF(OCF_CrewMember)), Find_Exclude(this()), Find_OCF(OCF_NotContained))){
			Message(Format("$HopperConnect$", GetName(ConnectedTo)), this());
			Sound("Click");
		}else{
			Message("$ConnectFail$",this());
			Sound("CommandFailure1");
		}
	}
}