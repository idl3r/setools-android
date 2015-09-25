class HelloWorld 
{
  public native void hacksepol();
  static 
  {
    System.loadLibrary("HelloWorldImp"); 
  }
}
