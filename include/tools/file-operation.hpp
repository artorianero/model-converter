#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>

int ReadDir(const std::string& path, std::vector<std::string>& files, bool shortname = true, bool recursive = false, std::vector<std::string> suffixs = {}) {
    int fileCount = 0;
    DIR* dir;
    struct dirent* dp;

    dir = opendir(path.c_str());
    if (dir == NULL) {
        printf("opendir %s failed\n", path.c_str());
        return 0;
    }

    while ((dp = readdir(dir)) != NULL) {
        std::string curpath(path);
        if (path.back() != '/') {
            curpath += std::string("/") += dp->d_name;
        } else {
            curpath += dp->d_name;
        }
        // 如果是目录，递归查找（需要判断recursive标志）
        if (dp->d_type == DT_DIR && recursive) {
            if (0 != strcmp(dp->d_name, ".") && 0 != strcmp(dp->d_name, "..")) {
                fileCount += ReadDir(curpath, files, shortname, recursive, suffixs);
            }
        }
        // 判断是否为文件以及文件后缀名
        else if (dp->d_type == DT_REG) {
            if (suffixs.size() <= 0) {
                if (shortname) {
                    std::size_t iPos = curpath.find_last_of('/') + 1;
                    files.push_back(curpath.substr(iPos, curpath.length() - iPos));
                } else {
                    files.push_back(curpath);
                }
                fileCount++;
            } else {
                for (auto suffix : suffixs) {
                    if (std::string(strrchr(dp->d_name, '.')) == suffix) {
                        if (shortname) {
                            std::size_t iPos = curpath.find_last_of('/') + 1;
                            files.push_back(curpath.substr(iPos, curpath.length() - iPos));
                        } else {
                            files.push_back(curpath);
                        }
                        fileCount++;
                        break;
                    }
                }
            }
        }
    }

    closedir(dir);
    return fileCount;
}

/**
 * Create floder if folder not exist
 */
void CheckFolder(const char* dir) {
    DIR* folder = opendir(dir);
    if (folder != NULL) {
        std::cout << "folder exists: " << dir << std::endl;
        return;
    } else {
        if (mkdir(dir, S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO) == 0) {
            std::cout << "create floder: " << dir << std::endl;
        }
    }
}

void FixDirPath(std::string* path) {
    if (path->at(path->length() - 1) != '/') {
        *path += "/";
    }
}
