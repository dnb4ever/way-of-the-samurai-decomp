import sys, io, re
import pycdlib
bin_path, outdir = sys.argv[1], sys.argv[2].rstrip('/')
iso_path = outdir + '/game.iso'
SECT, OFF, DATA = 2352, 24, 2048
n = 0
with open(bin_path, 'rb') as f, open(iso_path, 'wb') as o:
    while True:
        s = f.read(SECT)
        if len(s) < SECT: break
        o.write(s[OFF:OFF+DATA]); n += 1
print("stripped sectors:", n)
iso = pycdlib.PyCdlib(); iso.open(iso_path)
def read_file(*cands):
    last = None
    for p in cands:
        try:
            b = io.BytesIO(); iso.get_file_from_iso_fp(b, iso_path=p); return b.getvalue(), p
        except Exception as e: last = e
    raise last
print("=== root entries ===")
for c in iso.list_children(iso_path='/'):
    print("  ", c.file_identifier().decode('ascii','replace'))
cnf, cp = read_file('/SYSTEM.CNF;1', '/SYSTEM.CNF')
cnf = cnf.decode('ascii','replace')
print("=== SYSTEM.CNF (", cp, ") ==="); print(cnf.strip())
m = re.search(r'BOOT2\s*=\s*cdrom0:\\?([^;\s]+)', cnf)
exe = m.group(1).replace('\\','/').lstrip('/') if m else None
print("BOOT2 exe:", exe)
if exe:
    data, ep = read_file('/'+exe+';1', '/'+exe)
    dst = outdir + '/' + exe.split('/')[-1]
    open(dst,'wb').write(data)
    print("extracted:", dst, len(data), "bytes from", ep)
iso.close()
