#strict

func Initialize(){
  ScheduleCall(this,"Redefine",1);
}

func Redefine(){
  var real = CreateObject(CLBT);
  if(Contained()) Enter(Contained(),real);
  SetCommand(real,"Exit");
  return(RemoveObject());
}