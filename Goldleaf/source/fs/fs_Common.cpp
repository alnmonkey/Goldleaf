
/*

    Goldleaf - Multipurpose homebrew tool for Nintendo Switch
    Copyright © 2018-2025 XorTroll

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <fs/fs_FileSystem.hpp>

namespace fs {

    namespace {

        constexpr const char *SizeSuffixes[] = { "bytes", "KB", "MB", "GB", "TB", "PB", "EB" };

    }

    void CopyFileProgress(const std::string &path, const std::string &new_path, CopyFileStartCallback start_cb, CopyFileProgressCallback prog_cb) {
        auto exp = GetExplorerForPath(path);
        auto new_exp = GetExplorerForPath(new_path);

        auto sd_exp = GetSdCardExplorer();
        const auto file_size = exp->GetFileSize(path);
        if((file_size >= 4_GB) && (new_exp == sd_exp)) {
            CreateConcatenationFile(new_path);
        }
        exp->CopyFileProgress(path, new_path, start_cb, prog_cb);
    }

    void CopyDirectoryProgress(const std::string &dir, const std::string &new_dir, CopyDirectoryStartCallback start_cb, CopyDirectoryFileStartCallback file_start_cb, CopyDirectoryFileProgressCallback file_prog_cb) {
        auto exp = GetExplorerForPath(dir);
        exp->CopyDirectoryProgress(dir, new_dir, start_cb, file_start_cb, file_prog_cb);
    }

    u64 GetTotalSpaceForPartition(const Partition partition) {
        fs::Explorer *exp = nullptr;
        switch(partition) {
            case Partition::PRODINFOF: {
                exp = fs::GetPRODINFOFExplorer();
                break;
            }
            case Partition::NANDSafe: {
                exp = fs::GetNANDSafeExplorer();
                break;
            }
            case Partition::NANDSystem: {
                exp = fs::GetNANDSystemExplorer();
                break;
            }
            case Partition::NANDUser: {
                exp = fs::GetNANDUserExplorer();
                break;
            }
            case Partition::SdCard: {
                exp = fs::GetSdCardExplorer();
                break;
            }
        }
        if(exp != nullptr) {
            return exp->GetTotalSpace();
        }
        return 0;
    }

    u64 GetFreeSpaceForPartition(const Partition partition) {
        fs::Explorer *exp = nullptr;
        switch(partition) {
            case Partition::PRODINFOF: {
                exp = fs::GetPRODINFOFExplorer();
                break;
            }
            case Partition::NANDSafe: {
                exp = fs::GetNANDSafeExplorer();
                break;
            }
            case Partition::NANDSystem: {
                exp = fs::GetNANDSystemExplorer();
                break;
            }
            case Partition::NANDUser: {
                exp = fs::GetNANDUserExplorer();
                break;
            }
            case Partition::SdCard: {
                exp = fs::GetSdCardExplorer();
                break;
            }
        }
        if(exp != nullptr) {
            return exp->GetFreeSpace();
        }
        return 0;
    }

    std::string FormatSize(const u64 bytes) {
        if(bytes == 0) {
            return std::string("0") + " " + SizeSuffixes[0];
        }

        const auto plc = static_cast<u32>(floor(log(bytes) / log(1024)));
        const auto byte_num = (double)(bytes / pow(1024, plc));
        const double rbt = ((int)(byte_num * 100.0) / 100.0);
        std::stringstream strm;
        strm << rbt;
        return strm.str() + " " + SizeSuffixes[plc];
    }

}
