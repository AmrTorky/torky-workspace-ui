with open("index.html", "r", encoding="utf-8") as f:
    code = f.read()

# تصحيح دالة توليد أمر التيرمينال ليشير للمسار الصحيح للمحرك
old_gen_func = """function generateTerminalPayload() {
  const pVal = Number(sliderState.p_cub ?? -3.0).toFixed(2);
  const qVal = Number(sliderState.q_cub ?? 2.0).toFixed(2);
  const ecVal = Number(sliderState.ec ?? 124.8).toFixed(1);
  const bftVal = Number(sliderState.bft ?? 62);
  const tsgHex = "0x0" + tsgState.toString(16);
  const shmHex = "0x" + (BigInt(0x14f46b04000) + BigInt((currentLabId - 1) * 9)).toString(16);

  const cmd = `./${activeEngine.name} --lab ${String(currentLabId).padStart(2,'0')} --p ${pVal} --q ${qVal} --ec ${ecVal} --bft-us ${bftVal} --shm-ptr ${shmHex} --tsg ${tsgHex} --v-finite 11.384`;
  const preview = document.getElementById("cli-command-preview");
  if (preview) preview.textContent = cmd;
  logTerminal(`  [CLI PAYLOAD] ${cmd}`);
  return cmd;
}"""

new_gen_func = """function generateTerminalPayload() {
  const pVal = Number(sliderState.p_cub ?? -3.0).toFixed(2);
  const qVal = Number(sliderState.q_cub ?? 2.0).toFixed(2);
  const ecVal = Number(sliderState.ec ?? 124.8).toFixed(1);
  const bftVal = Number(sliderState.bft ?? 62);
  const tsgHex = "0x0" + tsgState.toString(16);
  const shmHex = "0x" + (BigInt(0x14f46b04000) + BigInt((currentLabId - 1) * 9)).toString(16);

  // توجيه المسار التنفيذي للمحرك الصحيح في جذر المنظومة أو تشغيله عبر بايثون السابورتر
  const cmd = `cd ~/dad_phone/torky_enterprise_fabric && ./${activeEngine.name}/build/${activeEngine.name} --lab ${String(currentLabId).padStart(2,'0')} --p ${pVal} --q ${qVal} --ec ${ecVal} --bft-us ${bftVal} --shm-ptr ${shmHex} --tsg ${tsgHex} --v-finite 11.384 || python3 ${activeEngine.name}/custom_input_runner.py`;
  const preview = document.getElementById("cli-command-preview");
  if (preview) preview.textContent = cmd;
  logTerminal(`  [CLI PAYLOAD] ${cmd}`);
  return cmd;
}"""

if old_gen_func in code:
    code = code.replace(old_gen_func, new_gen_func)
    with open("index.html", "w", encoding="utf-8") as f:
        f.write(code)
    print("SUCCESS: CLI payload path corrected successfully!")
else:
    print("WARNING: Exact match for generateTerminalPayload not found, updating via regex.")
    import re
    code = re.sub(r'function generateTerminalPayload\(\) \{[\s\S]*?^}', new_gen_func, code, flags=re.MULTILINE)
    with open("index.html", "w", encoding="utf-8") as f:
        f.write(code)
    print("SUCCESS: Updated via regex pattern match!")
