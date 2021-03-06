//-------------------------------------------------------------------------------------------------------
//	Copyright 2005 Claes Johanson & Vember Audio
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "vstcontrols.h"
#include "SurgeStorage.h"
#include "CDIBitmap.h"
#include "DspUtilities.h"

class CLFOGui : public CControl
{
public:
   const static int margin = 2;
   const static int margin2 = 7;
   const static int lpsize = 50;
   const static int scale = 18;
   const static int shadowoffset = 1;
   const static int skugga = 0xff5d5d5d;
   const static int splitpoint = lpsize + 20;

   CLFOGui(const CRect& size,
           bool trigmaskedit,
           IControlListener* listener = 0,
           long tag = 0,
           LFOStorage* lfodata = 0,
           SurgeStorage* storage = 0,
           StepSequencerStorage* ss = 0)
       : CControl(size, listener, tag, 0)
   {
      this->lfodata = lfodata;
      this->storage = storage;
      this->ss = ss;
      edit_trigmask = trigmaskedit;
      cdisurf = new CDIBitmap(getWidth() - splitpoint, getHeight());
      controlstate = 0;

      int bgcol = 0xffff9000;
      int fgcol = 0xff000000;
      float f_bgcol[4], f_fgcol[4];
      const float sc = (1.f / 255.f);
      f_bgcol[0] = (bgcol & 0xff) * sc;
      f_fgcol[0] = (fgcol & 0xff) * sc;
      f_bgcol[1] = ((bgcol >> 8) & 0xff) * sc;
      f_fgcol[1] = ((fgcol >> 8) & 0xff) * sc;
      f_bgcol[2] = ((bgcol >> 16) & 0xff) * sc;
      f_fgcol[2] = ((fgcol >> 16) & 0xff) * sc;

      f_bgcol[0] = powf(f_bgcol[0], 2.2f);
      f_bgcol[1] = powf(f_bgcol[1], 2.2f);
      f_bgcol[2] = powf(f_bgcol[2], 2.2f);
      f_fgcol[0] = powf(f_fgcol[0], 2.2f);
      f_fgcol[1] = powf(f_fgcol[1], 2.2f);
      f_fgcol[2] = powf(f_fgcol[2], 2.2f);

      for (int i = 0; i < 256; i++)
      {
         float x = i * sc;
         // unsigned int a = limit_range((unsigned int)((float)255.f*powf(x,1.f/2.2f)),0,255);
         unsigned int r = limit_range(
             (int)((float)255.f * powf(x * f_fgcol[0] + (1.f - x) * f_bgcol[0], 1.f / 2.2f)), 0,
             255);
         unsigned int g = limit_range(
             (int)((float)255.f * powf(x * f_fgcol[1] + (1.f - x) * f_bgcol[1], 1.f / 2.2f)), 0,
             255);
         unsigned int b = limit_range(
             (int)((float)255.f * powf(x * f_fgcol[2] + (1.f - x) * f_bgcol[2], 1.f / 2.2f)), 0,
             255);
         unsigned int a = 0xff;

         coltable[i] = r | (g << 8) | (b << 16) | (a << 24);
      }
   }
   // virtual void mouse (CDrawContext *pContext, CPoint &where, long buttons = -1);
   virtual CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons);
   virtual CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons);
   virtual CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons);

   virtual ~CLFOGui()
   {
      delete cdisurf;
   }
   virtual void draw(CDrawContext* dc);

protected:
   LFOStorage* lfodata;
   StepSequencerStorage* ss;
   SurgeStorage* storage;
   unsigned int coltable[256];
   CDIBitmap* cdisurf;
   CRect shaperect[n_lfoshapes];
   CRect steprect[n_stepseqsteps];
   CRect gaterect[n_stepseqsteps];
   CRect rect_ls, rect_le, rect_shapes, rect_steps, rect_steps_retrig;
   CRect ss_shift_left, ss_shift_right;
   bool edit_trigmask;
   int controlstate;

   CLASS_METHODS(CLFOGui, CControl)
};