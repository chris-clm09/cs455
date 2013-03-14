
/**********************************************************
* This call will represent a light source. 
**********************************************************/
class Light
{
public:
   /**********************************************************
   * Constructor
   **********************************************************/
   Light(vector4 deffuseColor, vector4 ambientColor,
         vector4 position, vector4 shine, bool enabled)
   : deffuseColor(deffuseColor),
     ambientColor(ambientColor),
     position(position),
     shine(shine),
     enabled(enabled)
   {
      return;
   }
   
   /**********************************************************
   * Default Constructor
   **********************************************************/
   Light(): deffuseColor(1,1,1,1), ambientColor(0,0,0,1),
     position(0,0,1,0), shine(0,0,0,0), enabled(false)
   {
      return;
   }
   
   void setDeffuseColor(const vector4 &defColor) { deffuseColor = defColor; }
   void setAmbientColor(const vector4 &ambColor) { ambientColor = ambColor; }
   void setPosition(const vector4 &setPosition)  { position     = setPosition; }   
   void setEnabled(bool enabledVal)              { enabled      = enabledVal;}
   
   //Member DATA
   vector4 deffuseColor;
   vector4 ambientColor;
   vector4 position;
   vector4 shine;
   bool enabled;
};
