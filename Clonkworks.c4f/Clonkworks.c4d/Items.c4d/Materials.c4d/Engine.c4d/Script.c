#strict

func Initialize() {
  return(1);
}

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}

public func IsAdvancedProduct() { return(1); }
public func IsAnvilProduct() { return(1); }