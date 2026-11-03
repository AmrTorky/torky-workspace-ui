import re

with open("index.html", "r", encoding="utf-8") as f:
    content = f.read()

# استبدال دالة الرسم القديمة بدالة تفاعلية مرتبطة بالمنزلقات والمعاملات الحية
old_render_func = """function renderRiemannianManifold3D() {
  const canvas = document.getElementById("manifold-canvas");
  const ctx = canvas.getContext("2d");
  if (canvas.width !== canvas.clientWidth || canvas.height !== canvas.clientHeight) {
    canvas.width = canvas.clientWidth;
    canvas.height = canvas.clientHeight;
  }
  const w = canvas.width, h = canvas.height;
  ctx.fillStyle = "#020408";
  ctx.fillRect(0, 0, w, h);

  if (tsgState !== 0x03) {
    phaseTick += 0.035 * timeDirection;
  }

  // Render 3D Isometric Riemannian Potential Surface Mesh V_finite(r)
  const cols = 22, rows = 14;
  const cx = w * 0.5, cy = h * 0.56;
  const scaleX = w / 34, scaleY = h / 32;

  for (let r = 0; r < rows - 1; r++) {
    ctx.beginPath();
    ctx.strokeStyle = r % 2 === 0 ? "rgba(0, 242, 254, 0.65)" : "rgba(16, 185, 129, 0.55)";
    ctx.lineWidth = 1.25;
    for (let c = 0; c < cols; c++) {
      const gx = c - cols / 2;
      const gy = r - rows / 2;
      const rad = Math.sqrt(gx * gx + gy * gy) * 0.18;
      // Non-singular origin capped wave modulated by Riemannian G1 gradient
      const vCap = torkyComputeVFinite(rad, false);
      const z = Math.sin(rad * 3.0 - phaseTick) * (vCap * 4.2 * chaosAmp) + Math.cos(gx * 0.4 + phaseTick) * 6.0;

      const isoX = cx + (gx - gy) * scaleX;
      const isoY = cy + (gx + gy) * (scaleY * 0.55) - z;
      if (c === 0) ctx.moveTo(isoX, isoY); else ctx.lineTo(isoX, isoY);
    }
    ctx.stroke();
  }

  requestAnimationFrame(renderRiemannianManifold3D);
}"""

new_render_func = """function renderRiemannianManifold3D() {
  const canvas = document.getElementById("manifold-canvas");
  const ctx = canvas.getContext("2d");
  if (!canvas) return;
  if (canvas.width !== canvas.clientWidth || canvas.height !== canvas.clientHeight) {
    canvas.width = canvas.clientWidth;
    canvas.height = canvas.clientHeight;
  }
  const w = canvas.width, h = canvas.height;
  ctx.fillStyle = "#020408";
  ctx.fillRect(0, 0, w, h);

  if (tsgState !== 0x03) {
    // ربط سرعة واتجاه النبض بمعاملات الكاردانو ومنزلقات التشتت
    const pMod = Math.abs(sliderState.p_cub || 3.0);
    phaseTick += (0.02 + pMod * 0.005) * timeDirection;
  }

  // ربط الأشكال الحية بالمعاملات النشطة للمنزلقات والمختبرات
  const cols = 24, rows = 16;
  const cx = w * 0.5, cy = h * 0.55;
  const scaleX = w / 36, scaleY = h / 34;
  const qVal = sliderState.q_cub || 2.0;
  const banachScale = sliderState.banach || 0.3849;
  const braidPhase = sliderState.braid || 0.7854;

  for (let r = 0; r < rows - 1; r++) {
    ctx.beginPath();
    // تدرج لوني ديناميكي حسب رقم المختبر وحالة TSG Gate
    const isVeto = (tsgState === 0x03);
    const strokeColor = isVeto ? (r % 2 === 0 ? "rgba(239, 68, 68, 0.75)" : "rgba(248, 113, 113, 0.6)") :
                        (currentLabId >= 20 ? "rgba(139, 92, 246, 0.7)" : (r % 2 === 0 ? "rgba(0, 242, 254, 0.7)" : "rgba(16, 185, 129, 0.6))"));
    ctx.strokeStyle = strokeColor;
    ctx.lineWidth = 1.35;

    for (let c = 0; c < cols; c++) {
      const gx = c - cols / 2;
      const gy = r - rows / 2;
      const rad = Math.sqrt(gx * gx + gy * gy) * 0.17;
      const vCap = torkyComputeVFinite(rad, false);

      // تعديل هندسي حي يستجيب لـ Cardano p/q، ومعامل Banach، وضفائر Anyon Braid
      const waveMod = Math.sin(rad * (2.0 + qVal * 0.2) - phaseTick + braidPhase);
      const z = waveMod * (vCap * 4.0 * chaosAmp * banachScale) + Math.cos(gx * 0.35 + phaseTick * 1.2) * (5.0 + currentLabId * 0.15);

      const isoX = cx + (gx - gy) * scaleX;
      const isoY = cy + (gx + gy) * (scaleY * 0.52) - z;
      if (c === 0) ctx.moveTo(isoX, isoY); else ctx.lineTo(isoX, isoY);
    }
    ctx.stroke();
  }

  requestAnimationFrame(renderRiemannianManifold3D);
}"""

if old_render_func in content:
    content = content.replace(old_render_func, new_render_func)
    with open("index.html", "w", encoding="utf-8") as f:
        f.write(content)
    print("SUCCESS: index.html manifold renderer updated dynamically!")
else:
    print("WARNING: Exact block not found, check markup.")
