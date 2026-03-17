#strict

func Initialize(){
  ScheduleCall(this,"Redefine",1);
}

func Redefine(){
  var real = CreateObject(CLBT,0,10);
  if(Contained()) Enter(Contained(),real);
  SetCommand(real,"Exit");
  return(RemoveObject());
}