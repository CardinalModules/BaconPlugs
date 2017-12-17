#include "BaconPlugs.hpp"
#include <jansson.h>

struct DMPText // Thanks http://scruss.com/blog/tag/font/
{
  typedef std::map< char, std::vector< bool > > fontData_t;
private:
  static fontData_t fontData;
  static bool initialized;
  void init();
  
public:
  DMPText() {
    init();
  }
  
  void drawText( NVGcontext *vg, Vec pos, char c, int pxSize )
  {
    fontData_t::iterator k = fontData.find( c );
    if( k != fontData.end() ) {
      fontData_t::mapped_type blist = k->second;
      int row=0, col=0;
      for( auto v = blist.begin(); v != blist.end(); ++v )
        {
          if( *v )
            {
              float xo = (col+0.5) * pxSize + pos.x;
              float yo = (row+0.5) * pxSize + pos.y;
              nvgBeginPath( vg );
              nvgRect( vg, xo, yo, pxSize, pxSize );
              nvgFillColor( vg, COLOR_BLACK );
              nvgFill( vg );
            }
          
          col++;
          if( col == 5 ) {
            col = 0;
            row ++;
          }
        }
    }
    else {
    }
  }
};


bool DMPText::initialized = false;
DMPText::fontData_t DMPText::fontData = DMPText::fontData_t();

void DMPText::init()
{
  if( ! initialized ) {
      initialized = true;
      printf( "%s\n", assetPlugin( plugin, "res/Keypunch029.json" ).c_str() );

      json_t *json;
      json_error_t error;
      
      json = json_load_file(assetPlugin( plugin, "res/Keypunch029.json" ).c_str(), 0, &error);
      if(!json) {
        printf( "JSON FILE not loaded\n" );
      }
      const char* key;
      json_t *value;
      json_object_foreach( json, key, value ) {
        fontData_t::mapped_type valmap;
        size_t index;
        json_t *aval;
        json_array_foreach( value, index, aval ) {
          std::string s( json_string_value( aval ) );
          for( const char* c = s.c_str(); *c != 0; ++c ) {
            valmap.push_back( *c == '#' );
          }
        }
        DMPText::fontData[ key[ 0 ] ] = valmap;
      }
  }
}


void DMPTextPanel::draw( NVGcontext *vg )
{
  DMPText d;

  nvgBeginPath( vg );
  nvgRect( vg, 0, 0, box.size.x, box.size.y );
  nvgFillColor( vg, nvgRGBA( 255, 255, 200, 255 ) );
  nvgFill( vg );

  Vec cpos = Vec( 0, 0 );
  for( const char* c = txt.c_str(); *c != 0; ++c ) {
    d.drawText( vg, cpos, *c, pxper );
    cpos.x += pxper * 5;
  }
}