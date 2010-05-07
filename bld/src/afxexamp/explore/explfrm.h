#ifndef _EXPLFRM_H_INCLUDED
#define _EXPLFRM_H_INCLUDED

class CExploreFrameWnd : public CFrameWnd {
    DECLARE_DYNCREATE( CExploreFrameWnd )

public:
    CExploreFrameWnd();

    virtual BOOL    OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext *pContext );

protected:
    CToolBar        m_wndToolBar;
    CStatusBar      m_wndStatusBar;
    CSplitterWnd    m_wndSplitter;

    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    DECLARE_MESSAGE_MAP()
};

#endif /* _EXPLFRM_H_INCLUDED */
