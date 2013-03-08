
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
         vector4 position, bool enabled)
   : deffuseColor(deffuseColor),
     ambientColor(ambientColor),
     position(position),
     enabled(enabled)
   {
      return;
   }
   
   /**********************************************************
   * Default Constructor
   **********************************************************/
   Light(): deffuseColor(1,1,1,1), ambientColor(1,1,1,1),
     position(0,0,1,0), enabled(false)
   {
      return;
   }
   
   void setDeffuseColor(vector4 &defColor) { deffuseColor = defColor; }
   void setAmbientColor(vector4 &ambColor) { ambientColor = ambColor; }
   void setPosition(vector4 &setPosition)  { position     = setPosition; }   
   void setEnabled(bool enabledVal)        { enabled      = enabledVal;}
   
   //Member DATA
   vector4 deffuseColor;
   vector4 ambientColor;
   vector4 position;
   bool enabled;
};
