/*
 *   Copyright © 2010 Fredrik Höglund <fredrik@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; see the file COPYING.  if not, write to
 *   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *   Boston, MA 02110-1301, USA.
 */

#ifndef BLUR_H
#define BLUR_H

#include <kwineffects.h>
#include <kwinglplatform.h>
#include <kwinglutils.h>

#include <QVector>
#include <QVector2D>

namespace KWayland
{
namespace Server
{
class BlurManagerInterface;
}
}

namespace KWin
{

class BlurShader;

class BlurEffect : public KWin::Effect
{
    Q_OBJECT
    Q_PROPERTY(int blurRadius READ blurRadius)
    Q_PROPERTY(bool cacheTexture READ isCacheTexture)
public:
    BlurEffect();
    ~BlurEffect();

    static bool supported();
    static bool enabledByDefault();

    void reconfigure(ReconfigureFlags flags) Q_DECL_OVERRIDE;
    void prePaintScreen(ScreenPrePaintData &data, int time) Q_DECL_OVERRIDE;
    void prePaintWindow(EffectWindow* w, WindowPrePaintData& data, int time) Q_DECL_OVERRIDE;
    void drawWindow(EffectWindow *w, int mask, QRegion region, WindowPaintData &data) Q_DECL_OVERRIDE;
    void paintEffectFrame(EffectFrame *frame, QRegion region, double opacity, double frameOpacity) Q_DECL_OVERRIDE;

    // for dynamic setting extraction
    int blurRadius() const;
    bool isCacheTexture() const {
        return m_shouldCache;
    }
    bool provides(Feature feature) Q_DECL_OVERRIDE;

    int requestedEffectChainPosition() const override {
        return 75;
    }

public Q_SLOTS:
    void slotWindowAdded(KWin::EffectWindow *w);
    void slotWindowDeleted(KWin::EffectWindow *w);
    void slotPropertyNotify(KWin::EffectWindow *w, long atom);
    void slotScreenGeometryChanged();

private:
    QRect expand(const QRect &rect) const;
    QRegion expand(const QRegion &region) const;
    QRegion blurRegion(const EffectWindow *w) const;
    bool shouldBlur(const EffectWindow *w, int mask, const WindowPaintData &data) const;
    void updateBlurRegion(EffectWindow *w) const;
    void doSimpleBlur(EffectWindow *w, const float opacity, const QMatrix4x4 &screenProjection);
    void doBlur(const QRegion &shape, const QRect &screen, const float opacity, const QMatrix4x4 &screenProjection);
    void doCachedBlur(EffectWindow *w, const QRegion& region, const float opacity, const QMatrix4x4 &screenProjection);
    void uploadRegion(QVector2D *&map, const QRegion &region);
    void uploadGeometry(GLVertexBuffer *vbo, const QRegion &horizontal, const QRegion &vertical);

private:
    BlurShader *shader;
    GLShader *m_simpleShader;
    GLRenderTarget *target;
    GLTexture tex;
    long net_wm_blur_region;
    QRegion m_damagedArea; // keeps track of the area which has been damaged (from bottom to top)
    QRegion m_paintedArea; // actually painted area which is greater than m_damagedArea
    QRegion m_currentBlur; // keeps track of the currently blured area of non-caching windows(from bottom to top)
    bool m_shouldCache;

    struct BlurWindowInfo {
        GLTexture blurredBackground; // keeps the horizontally blurred background
        QRegion damagedRegion;
        QPoint windowPos;
        bool dropCache;
        QMetaObject::Connection blurChangedConnection;
    };

    QHash< const EffectWindow*, BlurWindowInfo > windows;
    typedef QHash<const EffectWindow*, BlurWindowInfo>::iterator CacheEntry;
    KWayland::Server::BlurManagerInterface *m_blurManager = nullptr;
};

inline
bool BlurEffect::provides(Effect::Feature feature)
{
    if (feature == Blur) {
        return true;
    }
    return KWin::Effect::provides(feature);
}


} // namespace KWin

#endif

