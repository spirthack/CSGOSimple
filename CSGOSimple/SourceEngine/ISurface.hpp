#pragma once

#include "Definitions.hpp"

#include "Color.hpp"
#include "IAppSystem.hpp"

namespace SourceEngine
{
     typedef ULONG HFont;

     struct IntRect {
          int x0;
          int y0;
          int w;
          int h;
     };

     struct Vertex_t {
          Vertex_t() {}
          Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0)) {
               m_Position = pos;
               m_TexCoord = coord;
          }
          void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0)) {
               m_Position = pos;
               m_TexCoord = coord;
          }

          Vector2D m_Position;
          Vector2D m_TexCoord;
     };

     #ifdef CreateFont
     #undef CreateFont
     #endif
     //-----------------------------------------------------------------------------
     // Purpose: Wraps contextless windows system functions
     //-----------------------------------------------------------------------------
     class ISurface : public IAppSystem {
     public:
          // frame
          virtual void        RunFrame() = 0;
          virtual VPANEL      GetEmbeddedPanel() = 0;
          virtual void        SetEmbeddedPanel(VPANEL pPanel) = 0;
          virtual void        PushMakeCurrent(VPANEL panel, bool useInsets) = 0;
          virtual void        PopMakeCurrent(VPANEL panel) = 0;
          virtual void        DrawSetColor(int r, int g, int b, int a) = 0;
          virtual void        DrawSetColor(Color col) = 0;
          virtual void        DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
          virtual void        DrawFilledRectArray(IntRect *pRects, int numRects) = 0;
          virtual void        DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
          virtual void        DrawLine(int x0, int y0, int x1, int y1) = 0;
          virtual void        DrawPolyLine(int *px, int *py, int numPoints) = 0;
          virtual void        DrawSetApparentDepth(float f) = 0;
          virtual void        DrawClearApparentDepth(void) = 0;
          virtual void        DrawSetTextFont(HFont font) = 0;
          virtual void        DrawSetTextColor(int r, int g, int b, int a) = 0;
          virtual void        DrawSetTextColor(Color col) = 0;
          virtual void        DrawSetTextPos(int x, int y) = 0;
          virtual void        DrawGetTextPos(int& x, int& y) = 0;
          virtual void        DrawPrintText(const wchar_t *text, int textLen, FontDrawType drawType = FontDrawType::FONT_DRAW_DEFAULT) = 0;
          virtual void        DrawUnicodeChar(wchar_t wch, FontDrawType drawType = FontDrawType::FONT_DRAW_DEFAULT) = 0;
          virtual void        DrawFlushText() = 0;
          virtual void*       CreateHTMLWindow(void *events, VPANEL context) = 0;
          virtual void        PaintHTMLWindow(void *htmlwin) = 0;
          virtual void        DeleteHTMLWindow(void *htmlwin) = 0;
          virtual int         DrawGetTextureId(char const *filename) = 0;
          virtual bool        DrawGetTextureFile(int id, char *filename, int maxlen) = 0;
          virtual void        DrawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload) = 0;
          virtual void        DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall) = 0;
          virtual void        DrawSetTexture(int id) = 0;
          virtual void        DeleteTextureByID(int id) = 0;
          virtual void        DrawGetTextureSize(int id, int &wide, int &tall) = 0;
          virtual void        DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
          virtual bool        IsTextureIDValid(int id) = 0;
          virtual int         CreateNewTextureID(bool procedural = false) = 0;
          virtual void        GetScreenSize(int &wide, int &tall) = 0;
          virtual void        SetAsTopMost(VPANEL panel, bool state) = 0;
          virtual void        BringToFront(VPANEL panel) = 0;
          virtual void        SetForegroundWindow(VPANEL panel) = 0;
          virtual void        SetPanelVisible(VPANEL panel, bool state) = 0;
          virtual void        SetMinimized(VPANEL panel, bool state) = 0;
          virtual bool        IsMinimized(VPANEL panel) = 0;
          virtual void        FlashWindow(VPANEL panel, bool state) = 0;
          virtual void        SetTitle(VPANEL panel, const wchar_t *title) = 0;
          virtual void        SetAsToolBar(VPANEL panel, bool state) = 0;
          virtual void        CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
          virtual void        SwapBuffers(VPANEL panel) = 0;
          virtual void        Invalidate(VPANEL panel) = 0;
          virtual void        SetCursor(unsigned long cursor) = 0;
          virtual bool        IsCursorVisible() = 0;
          virtual void        ApplyChanges() = 0;
          virtual bool        IsWithin(int x, int y) = 0;
          virtual bool        HasFocus() = 0;
          virtual bool        SupportsFeature(int /*SurfaceFeature_t*/ feature) = 0;
          virtual void        RestrictPaintToSinglePanel(VPANEL panel, bool bForceAllowNonModalSurface = false) = 0;
          virtual void        SetModalPanel(VPANEL) = 0;
          virtual VPANEL      GetModalPanel() = 0;
          virtual void        UnlockCursor() = 0;
          virtual void        LockCursor() = 0;
          virtual void        SetTranslateExtendedKeys(bool state) = 0;
          virtual VPANEL      GetTopmostPopup() = 0;
          virtual void        SetTopLevelFocus(VPANEL panel) = 0;
          virtual HFont       CreateFont() = 0;
          virtual bool        SetFontGlyphSet(HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) = 0;
          virtual bool        AddCustomFontFile(const char *fontFileName) = 0;
          virtual int         GetFontTall(HFont font) = 0;
          virtual int         GetFontAscent(HFont font, wchar_t wch) = 0;
          virtual bool        IsFontAdditive(HFont font) = 0;
          virtual void        GetCharABCwide(HFont font, int ch, int &a, int &b, int &c) = 0;
          virtual int         GetCharacterWidth(HFont font, int ch) = 0;
          virtual void        GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall) = 0;
          virtual VPANEL      GetNotifyPanel() = 0;
          virtual void        SetNotifyIcon(VPANEL context, ULONG icon, VPANEL panelToReceiveMessages, const char *text) = 0;
          virtual void        PlaySound(const char *fileName) = 0;
          virtual int         GetPopupCount() = 0;
          virtual VPANEL      GetPopup(int index) = 0;
          virtual bool        ShouldPaintChildPanel(VPANEL childPanel) = 0;
          virtual bool        RecreateContext(VPANEL panel) = 0;
          virtual void        AddPanel(VPANEL panel) = 0;
          virtual void        ReleasePanel(VPANEL panel) = 0;
          virtual void        MovePopupToFront(VPANEL panel) = 0;
          virtual void        MovePopupToBack(VPANEL panel) = 0;
          virtual void        SolveTraverse(VPANEL panel, bool forceApplySchemeSettings = false) = 0;
          virtual void        PaintTraverse(VPANEL panel) = 0;
          virtual void        EnableMouseCapture(VPANEL panel, bool state) = 0;
          virtual void        GetWorkspaceBounds(int &x, int &y, int &wide, int &tall) = 0;
          virtual void        GetAbsoluteWindowBounds(int &x, int &y, int &wide, int &tall) = 0;
          virtual void        GetProportionalBase(int &width, int &height) = 0;
          virtual void        CalculateMouseVisible() = 0;
          virtual bool        NeedKBInput() = 0;
          virtual bool        HasCursorPosFunctions() = 0;
          virtual void        SurfaceGetCursorPos(int &x, int &y) = 0;
          virtual void        SurfaceSetCursorPos(int x, int y) = 0; 
          virtual void        DrawTexturedLine(const Vertex_t &a, const Vertex_t &b) = 0;
          virtual void        DrawOutlinedCircle(int x, int y, int radius, int segments) = 0;
          virtual void        DrawTexturedPolyLine(const Vertex_t *p, int n) = 0;
          virtual void        DrawTexturedSubRect(int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1) = 0;
          virtual void        DrawTexturedPolygon(int n, Vertex_t *pVertice, bool bClipVertices = true) = 0;

     };
}