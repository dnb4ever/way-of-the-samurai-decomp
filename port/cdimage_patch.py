#!/usr/bin/env python3
# Scratch-tree-only (ps2xIOP): when a CD image is configured, build the cdvd directory tree from the
# image's real ISO9660 directory so sceCdSearchFile LBAs and sceCdRead (which already prefers the image)
# agree. Without an image the cdRoot-synthesised virtual ISO is used as before.
import os
import shutil
import sys

P = "/tmp/ps2recomp_build/PS2Recomp/ps2xIOP/src/emulator/imports/iop_cdvd.cpp"
if not os.path.exists(P + ".orig"):
    shutil.copyfile(P, P + ".orig")
s = open(P).read()

old = """        bool buildVirtualIso()
        {
            if (virtualIsoBuilt)
                return virtualIsoValid;
            virtualIsoBuilt = true;
"""
new = """        static uint32_t isoLe32(const uint8_t *bytes)
        {
            return static_cast<uint32_t>(bytes[0]) | (static_cast<uint32_t>(bytes[1]) << 8u) |
                   (static_cast<uint32_t>(bytes[2]) << 16u) | (static_cast<uint32_t>(bytes[3]) << 24u);
        }

        bool readImageSector(uint32_t lsn, uint8_t *destination)
        {
            const std::string imagePath = host.hostPath(HostPathKind::CdImage);
            if (imagePath.empty())
                return false;
            if (imageHandle == 0u)
                imageHandle = host.openHostFile(imagePath);
            if (imageHandle == 0u)
                return false;
            size_t bytesRead = 0u;
            return host.readHostFile(imageHandle, static_cast<uint64_t>(lsn) * kSectorSize, destination, kSectorSize, bytesRead) &&
                   bytesRead == kSectorSize;
        }

        // Walks one ISO9660 directory extent of the configured image into `nodes`.
        bool addImageDirectory(size_t parent, uint32_t lsn, uint32_t size, uint32_t depth)
        {
            if (depth > 8u || size == 0u || size > 64u * 1024u * 1024u)
                return false;
            const uint32_t sectors = (size + kSectorSize - 1u) / kSectorSize;
            std::vector<uint8_t> bytes(static_cast<size_t>(sectors) * kSectorSize);
            for (uint32_t sector = 0u; sector < sectors; ++sector)
            {
                if (!readImageSector(lsn + sector, bytes.data() + static_cast<size_t>(sector) * kSectorSize))
                    return false;
            }
            size_t offset = 0u;
            while (offset + 33u <= bytes.size())
            {
                const uint8_t length = bytes[offset];
                if (length == 0u)
                {
                    offset = (offset / kSectorSize + 1u) * kSectorSize; // records never straddle sectors
                    continue;
                }
                if (offset + length > bytes.size() || length < 33u)
                    break;
                const uint32_t extent = isoLe32(bytes.data() + offset + 2u);
                const uint32_t dataSize = isoLe32(bytes.data() + offset + 10u);
                const uint8_t flags = bytes[offset + 25u];
                const uint8_t nameLength = bytes[offset + 32u];
                const std::string name(reinterpret_cast<const char *>(bytes.data() + offset + 33u),
                                       std::min<size_t>(nameLength, length - 33u));
                offset += length;
                if (nameLength == 1u && (name[0] == '\\0' || name[0] == '\\1'))
                    continue; // "." and ".."
                IsoNode node;
                node.identifier = name;
                node.parent = parent;
                node.directory = (flags & 2u) != 0u;
                node.lsn = extent;
                node.size = dataSize;
                node.sectors = (dataSize + kSectorSize - 1u) / kSectorSize;
                const size_t index = nodes.size();
                nodes.push_back(std::move(node));
                nodes[parent].children.push_back(index);
                if (nodes[index].directory && !addImageDirectory(index, extent, dataSize, depth + 1u))
                    return false;
            }
            return true;
        }

        bool buildImageIso()
        {
            std::array<uint8_t, kSectorSize> primary{};
            if (!readImageSector(16u, primary.data()) || std::memcmp(primary.data() + 1u, "CD001", 5u) != 0)
                return false;
            const uint32_t rootLsn = isoLe32(primary.data() + 156u + 2u);
            const uint32_t rootSize = isoLe32(primary.data() + 156u + 10u);
            volumeSectors = isoLe32(primary.data() + 80u);
            nodes.clear();
            metadataSectors.clear();
            IsoNode root;
            root.directory = true;
            root.parent = 0u;
            root.lsn = rootLsn;
            root.size = rootSize;
            root.sectors = (rootSize + kSectorSize - 1u) / kSectorSize;
            nodes.push_back(std::move(root));
            if (!addImageDirectory(0u, rootLsn, rootSize, 0u))
            {
                nodes.clear();
                return false;
            }
            std::ostringstream out;
            out << "[IOP cdvd] directory built from CD image: " << nodes.size() - 1u << " entries, volume " << volumeSectors << " sectors";
            host.log(LogLevel::Info, out.str());
            return true;
        }

        bool buildVirtualIso()
        {
            if (virtualIsoBuilt)
                return virtualIsoValid;
            virtualIsoBuilt = true;
            if (!host.hostPath(HostPathKind::CdImage).empty() && buildImageIso())
            {
                virtualIsoValid = true;
                return true;
            }
"""
if s.count(old) != 1:
    print("FAIL anchor")
    sys.exit(1)
s = s.replace(old, new)
if "#include <sstream>" not in s:
    s = s.replace("#include <algorithm>", "#include <algorithm>\n#include <sstream>", 1) if "#include <algorithm>" in s else "#include <sstream>\n" + s
open(P, "w").write(s)
print("patched", P)
