package com.hekangyuan.nivi1000.file

import android.content.Context
import android.net.Uri
import android.os.Build
import android.os.StatFs
import android.text.TextUtils
import java.io.*
import java.net.URL
import java.security.DigestInputStream
import java.security.MessageDigest
import java.security.NoSuchAlgorithmException
import java.util.*
import javax.net.ssl.HttpsURLConnection

/**
 * <pre>
 * author: Blankj
 * blog  : http://blankj.com
 * time  : 2016/05/03
 * desc  : utils about file
</pre> *
 */
class FileUtils private constructor() {
    ///////////////////////////////////////////////////////////////////////////
    // interface
    ///////////////////////////////////////////////////////////////////////////
    interface OnReplaceListener {
        fun onReplace(srcFile: File?, destFile: File?): Boolean
    }

    companion object {
        private val LINE_SEP = System.getProperty("line.separator")

        /**
         * Return the file by path.
         *
         * @param filePath The path of file.
         * @return the file
         */
        fun getFileByPath(filePath: String?): File? {
            return if (StringUtils.isSpace(filePath)) null else File(filePath)
        }

        /**
         * Return whether the file exists.
         *
         * @param file The file.
         * @return `true`: yes<br></br>`false`: no
         */
        fun isFileExists(file: File?, context: Context): Boolean {
            if (file == null) return false
            return if (file.exists()) {
                true
            } else isFileExists(file.absolutePath, context)
        }

        /**
         * Return whether the file exists.
         *
         * @param filePath The path of file.
         * @return `true`: yes<br></br>`false`: no
         */
        fun isFileExists(filePath: String?, context: Context): Boolean {
            val file = getFileByPath(filePath) ?: return false
            return if (file.exists()) {
                true
            } else isFileExistsApi29(filePath, context)
        }

        private fun isFileExistsApi29(filePath: String?, context: Context): Boolean {
            if (Build.VERSION.SDK_INT >= 29) {
                try {
                    val uri = Uri.parse(filePath)
                    val cr = context.applicationContext.contentResolver
                    val afd = cr.openAssetFileDescriptor(uri, "r") ?: return false
                    try {
                        afd.close()
                    } catch (ignore: IOException) {
                    }
                } catch (e: FileNotFoundException) {
                    return false
                }
                return true
            }
            return false
        }

        /**
         * Rename the file.
         *
         * @param filePath The path of file.
         * @param newName  The new name of file.
         * @return `true`: success<br></br>`false`: fail
         */
        fun rename(filePath: String?, newName: String): Boolean {
            return rename(getFileByPath(filePath), newName)
        }

        /**
         * Rename the file.
         *
         * @param file    The file.
         * @param newName The new name of file.
         * @return `true`: success<br></br>`false`: fail
         */
        fun rename(file: File?, newName: String): Boolean {
            // file is null then return false
            if (file == null) return false
            // file doesn't exist then return false
            if (!file.exists()) return false
            // the new name is space then return false
            if (StringUtils.isSpace(newName)) return false
            // the new name equals old name then return true
            if (newName == file.name) return true
            val newFile = File(file.parent + File.separator + newName)
            // the new name of file exists then return false
            return (!newFile.exists()
                    && file.renameTo(newFile))
        }

        /**
         * Return whether it is a directory.
         *
         * @param dirPath The path of directory.
         * @return `true`: yes<br></br>`false`: no
         */
        fun isDir(dirPath: String?): Boolean {
            return isDir(getFileByPath(dirPath))
        }

        /**
         * Return whether it is a directory.
         *
         * @param file The file.
         * @return `true`: yes<br></br>`false`: no
         */
        fun isDir(file: File?): Boolean {
            return file != null && file.exists() && file.isDirectory
        }

        /**
         * Return whether it is a file.
         *
         * @param filePath The path of file.
         * @return `true`: yes<br></br>`false`: no
         */
        fun isFile(filePath: String?): Boolean {
            return isFile(getFileByPath(filePath))
        }

        /**
         * Return whether it is a file.
         *
         * @param file The file.
         * @return `true`: yes<br></br>`false`: no
         */
        fun isFile(file: File?): Boolean {
            return file != null && file.exists() && file.isFile
        }

        /**
         * Create a directory if it doesn't exist, otherwise do nothing.
         *
         * @param dirPath The path of directory.
         * @return `true`: exists or creates successfully<br></br>`false`: otherwise
         */
        fun createOrExistsDir(dirPath: String?): Boolean {
            return createOrExistsDir(getFileByPath(dirPath))
        }

        /**
         * Create a directory if it doesn't exist, otherwise do nothing.
         *
         * @param file The file.
         * @return `true`: exists or creates successfully<br></br>`false`: otherwise
         */
        fun createOrExistsDir(file: File?): Boolean {
            return file != null && if (file.exists()) file.isDirectory else file.mkdirs()
        }

        /**
         * Create a file if it doesn't exist, otherwise do nothing.
         *
         * @param filePath The path of file.
         * @return `true`: exists or creates successfully<br></br>`false`: otherwise
         */
        fun createOrExistsFile(filePath: String?): Boolean {
            return createOrExistsFile(getFileByPath(filePath))
        }

        /**
         * Create a file if it doesn't exist, otherwise do nothing.
         *
         * @param file The file.
         * @return `true`: exists or creates successfully<br></br>`false`: otherwise
         */
        fun createOrExistsFile(file: File?): Boolean {
            if (file == null) return false
            if (file.exists()) return file.isFile
            return if (!createOrExistsDir(file.parentFile)) false else try {
                file.createNewFile()
            } catch (e: IOException) {
                e.printStackTrace()
                false
            }
        }

        /**
         * Create a file if it doesn't exist, otherwise delete old file before creating.
         *
         * @param filePath The path of file.
         * @return `true`: success<br></br>`false`: fail
         */
        fun createFileByDeleteOldFile(filePath: String?): Boolean {
            return createFileByDeleteOldFile(getFileByPath(filePath))
        }

        /**
         * Create a file if it doesn't exist, otherwise delete old file before creating.
         *
         * @param file The file.
         * @return `true`: success<br></br>`false`: fail
         */
        fun createFileByDeleteOldFile(file: File?): Boolean {
            if (file == null) return false
            // file exists and unsuccessfully delete then return false
            if (file.exists() && !file.delete()) return false
            return if (!createOrExistsDir(file.parentFile)) false else try {
                file.createNewFile()
            } catch (e: IOException) {
                e.printStackTrace()
                false
            }
        }

        /**
         * Delete the directory.
         *
         * @param filePath The path of file.
         * @return `true`: success<br></br>`false`: fail
         */
        fun delete(filePath: String?): Boolean {
            return delete(getFileByPath(filePath))
        }

        /**
         * Delete the directory.
         *
         * @param file The file.
         * @return `true`: success<br></br>`false`: fail
         */
        fun delete(file: File?): Boolean {
            if (file == null) return false
            return if (file.isDirectory) {
                deleteDir(file)
            } else deleteFile(file)
        }

        /**
         * Delete the directory.
         *
         * @param dir The directory.
         * @return `true`: success<br></br>`false`: fail
         */
        private fun deleteDir(dir: File?): Boolean {
            if (dir == null) return false
            // dir doesn't exist then return true
            if (!dir.exists()) return true
            // dir isn't a directory then return false
            if (!dir.isDirectory) return false
            val files = dir.listFiles()
            if (files != null && files.size > 0) {
                for (file in files) {
                    if (file.isFile) {
                        if (!file.delete()) return false
                    } else if (file.isDirectory) {
                        if (!deleteDir(file)) return false
                    }
                }
            }
            return dir.delete()
        }

        /**
         * Delete the file.
         *
         * @param file The file.
         * @return `true`: success<br></br>`false`: fail
         */
        private fun deleteFile(file: File?): Boolean {
            return file != null && (!file.exists() || file.isFile && file.delete())
        }

        /**
         * Delete the all in directory.
         *
         * @param dirPath The path of directory.
         * @return `true`: success<br></br>`false`: fail
         */
        fun deleteAllInDir(dirPath: String?): Boolean {
            return deleteAllInDir(getFileByPath(dirPath))
        }

        /**
         * Delete the all in directory.
         *
         * @param dir The directory.
         * @return `true`: success<br></br>`false`: fail
         */
        fun deleteAllInDir(dir: File?): Boolean {
            return deleteFilesInDirWithFilter(dir, FileFilter { true })
        }

        /**
         * Delete all files in directory.
         *
         * @param dirPath The path of directory.
         * @return `true`: success<br></br>`false`: fail
         */
        fun deleteFilesInDir(dirPath: String?): Boolean {
            return deleteFilesInDir(getFileByPath(dirPath))
        }

        /**
         * Delete all files in directory.
         *
         * @param dir The directory.
         * @return `true`: success<br></br>`false`: fail
         */
        fun deleteFilesInDir(dir: File?): Boolean {
            return deleteFilesInDirWithFilter(dir, FileFilter { pathname -> pathname.isFile })
        }

        /**
         * Delete all files that satisfy the filter in directory.
         *
         * @param dirPath The path of directory.
         * @param filter  The filter.
         * @return `true`: success<br></br>`false`: fail
         */
        fun deleteFilesInDirWithFilter(dirPath: String?,
                                       filter: FileFilter?): Boolean {
            return deleteFilesInDirWithFilter(getFileByPath(dirPath), filter)
        }

        /**
         * Delete all files that satisfy the filter in directory.
         *
         * @param dir    The directory.
         * @param filter The filter.
         * @return `true`: success<br></br>`false`: fail
         */
        fun deleteFilesInDirWithFilter(dir: File?, filter: FileFilter?): Boolean {
            if (dir == null || filter == null) return false
            // dir doesn't exist then return true
            if (!dir.exists()) return true
            // dir isn't a directory then return false
            if (!dir.isDirectory) return false
            val files = dir.listFiles()
            if (files != null && files.size != 0) {
                for (file in files) {
                    if (filter.accept(file)) {
                        if (file.isFile) {
                            if (!file.delete()) return false
                        } else if (file.isDirectory) {
                            if (!deleteDir(file)) return false
                        }
                    }
                }
            }
            return true
        }
        /**
         * Return the files in directory.
         *
         * Doesn't traverse subdirectories
         *
         * @param dirPath    The path of directory.
         * @param comparator The comparator to determine the order of the list.
         * @return the files in directory
         */
        /**
         * Return the files in directory.
         *
         * Doesn't traverse subdirectories
         *
         * @param dirPath The path of directory.
         * @return the files in directory
         */
        @JvmOverloads
        fun listFilesInDir(dirPath: String?, comparator: Comparator<File?>? = null): List<File?> {
            return listFilesInDir(getFileByPath(dirPath), false, comparator)
        }
        /**
         * Return the files in directory.
         *
         * Doesn't traverse subdirectories
         *
         * @param dir        The directory.
         * @param comparator The comparator to determine the order of the list.
         * @return the files in directory
         */
        /**
         * Return the files in directory.
         *
         * Doesn't traverse subdirectories
         *
         * @param dir The directory.
         * @return the files in directory
         */
        @JvmOverloads
        fun listFilesInDir(dir: File?, comparator: Comparator<File?>? = null): List<File?> {
            return listFilesInDir(dir, false, comparator)
        }

        /**
         * Return the files in directory.
         *
         * @param dirPath     The path of directory.
         * @param isRecursive True to traverse subdirectories, false otherwise.
         * @return the files in directory
         */
        fun listFilesInDir(dirPath: String?, isRecursive: Boolean): List<File?> {
            return listFilesInDir(getFileByPath(dirPath), isRecursive)
        }

        /**
         * Return the files in directory.
         *
         * @param dirPath     The path of directory.
         * @param isRecursive True to traverse subdirectories, false otherwise.
         * @param comparator  The comparator to determine the order of the list.
         * @return the files in directory
         */
        fun listFilesInDir(dirPath: String?,
                           isRecursive: Boolean,
                           comparator: Comparator<File?>?): List<File?> {
            return listFilesInDir(getFileByPath(dirPath), isRecursive, comparator)
        }
        /**
         * Return the files in directory.
         *
         * @param dir         The directory.
         * @param isRecursive True to traverse subdirectories, false otherwise.
         * @param comparator  The comparator to determine the order of the list.
         * @return the files in directory
         */
        /**
         * Return the files in directory.
         *
         * @param dir         The directory.
         * @param isRecursive True to traverse subdirectories, false otherwise.
         * @return the files in directory
         */
        @JvmOverloads
        fun listFilesInDir(dir: File?,
                           isRecursive: Boolean,
                           comparator: Comparator<File?>? = null): List<File?> {
            return listFilesInDirWithFilter(dir, FileFilter { true }, isRecursive, comparator)
        }

        /**
         * Return the files that satisfy the filter in directory.
         *
         * Doesn't traverse subdirectories
         *
         * @param dirPath The path of directory.
         * @param filter  The filter.
         * @return the files that satisfy the filter in directory
         */
        fun listFilesInDirWithFilter(dirPath: String?,
                                     filter: FileFilter): List<File?> {
            return listFilesInDirWithFilter(getFileByPath(dirPath), filter)
        }

        /**
         * Return the files that satisfy the filter in directory.
         *
         * Doesn't traverse subdirectories
         *
         * @param dirPath    The path of directory.
         * @param filter     The filter.
         * @param comparator The comparator to determine the order of the list.
         * @return the files that satisfy the filter in directory
         */
        fun listFilesInDirWithFilter(dirPath: String?,
                                     filter: FileFilter,
                                     comparator: Comparator<File?>?): List<File?> {
            return listFilesInDirWithFilter(getFileByPath(dirPath), filter, comparator)
        }

        /**
         * Return the files that satisfy the filter in directory.
         *
         * Doesn't traverse subdirectories
         *
         * @param dir        The directory.
         * @param filter     The filter.
         * @param comparator The comparator to determine the order of the list.
         * @return the files that satisfy the filter in directory
         */
        fun listFilesInDirWithFilter(dir: File?,
                                     filter: FileFilter,
                                     comparator: Comparator<File?>?): List<File?> {
            return listFilesInDirWithFilter(dir, filter, false, comparator)
        }

        /**
         * Return the files that satisfy the filter in directory.
         *
         * @param dirPath     The path of directory.
         * @param filter      The filter.
         * @param isRecursive True to traverse subdirectories, false otherwise.
         * @return the files that satisfy the filter in directory
         */
        fun listFilesInDirWithFilter(dirPath: String?,
                                     filter: FileFilter,
                                     isRecursive: Boolean): List<File?> {
            return listFilesInDirWithFilter(getFileByPath(dirPath), filter, isRecursive)
        }

        /**
         * Return the files that satisfy the filter in directory.
         *
         * @param dirPath     The path of directory.
         * @param filter      The filter.
         * @param isRecursive True to traverse subdirectories, false otherwise.
         * @param comparator  The comparator to determine the order of the list.
         * @return the files that satisfy the filter in directory
         */
        fun listFilesInDirWithFilter(dirPath: String?,
                                     filter: FileFilter,
                                     isRecursive: Boolean,
                                     comparator: Comparator<File?>?): List<File?> {
            return listFilesInDirWithFilter(getFileByPath(dirPath), filter, isRecursive, comparator)
        }
        /**
         * Return the files that satisfy the filter in directory.
         *
         * @param dir         The directory.
         * @param filter      The filter.
         * @param isRecursive True to traverse subdirectories, false otherwise.
         * @param comparator  The comparator to determine the order of the list.
         * @return the files that satisfy the filter in directory
         */
        /**
         * Return the files that satisfy the filter in directory.
         *
         * Doesn't traverse subdirectories
         *
         * @param dir    The directory.
         * @param filter The filter.
         * @return the files that satisfy the filter in directory
         */
        /**
         * Return the files that satisfy the filter in directory.
         *
         * @param dir         The directory.
         * @param filter      The filter.
         * @param isRecursive True to traverse subdirectories, false otherwise.
         * @return the files that satisfy the filter in directory
         */
        @JvmOverloads
        fun listFilesInDirWithFilter(dir: File?,
                                     filter: FileFilter,
                                     isRecursive: Boolean = false,
                                     comparator: Comparator<File?>? = null): List<File?> {
            val files = listFilesInDirWithFilterInner(dir, filter, isRecursive)
            if (comparator != null) {
                Collections.sort(files, comparator)
            }
            return files
        }

        private fun listFilesInDirWithFilterInner(dir: File?,
                                                  filter: FileFilter,
                                                  isRecursive: Boolean): List<File?> {
            val list: MutableList<File?> = ArrayList()
            if (!isDir(dir)) return list
            val files = dir!!.listFiles()
            if (files != null && files.size > 0) {
                for (file in files) {
                    if (filter.accept(file)) {
                        list.add(file)
                    }
                    if (isRecursive && file.isDirectory) {
                        list.addAll(listFilesInDirWithFilterInner(file, filter, true))
                    }
                }
            }
            return list
        }

        /**
         * Return the time that the file was last modified.
         *
         * @param filePath The path of file.
         * @return the time that the file was last modified
         */
        fun getFileLastModified(filePath: String?): Long {
            return getFileLastModified(getFileByPath(filePath))
        }

        /**
         * Return the time that the file was last modified.
         *
         * @param file The file.
         * @return the time that the file was last modified
         */
        fun getFileLastModified(file: File?): Long {
            return file?.lastModified() ?: -1
        }


        /**
         * Return the length.
         *
         * @param filePath The path of file.
         * @return the length
         */
        fun getLength(filePath: String?): Long {
            return getLength(getFileByPath(filePath))
        }

        /**
         * Return the length.
         *
         * @param file The file.
         * @return the length
         */
        fun getLength(file: File?): Long {
            if (file == null) return 0
            return if (file.isDirectory) {
                getDirLength(file)
            } else getFileLength(file)
        }

        /**
         * Return the length of directory.
         *
         * @param dir The directory.
         * @return the length of directory
         */
        private fun getDirLength(dir: File): Long {
            if (!isDir(dir)) return -1
            var len: Long = 0
            val files = dir.listFiles()
            if (files != null && files.size > 0) {
                for (file in files) {
                    len += if (file.isDirectory) {
                        getDirLength(file)
                    } else {
                        file.length()
                    }
                }
            }
            return len
        }


        /**
         * Return the length of file.
         *
         * @param file The file.
         * @return the length of file
         */
        private fun getFileLength(file: File?): Long {
            return if (!isFile(file)) -1 else file!!.length()
        }

        /**
         * Return the total size of file system.
         *
         * @param anyPathInFs Any path in file system.
         * @return the total size of file system
         */
        fun getFsTotalSize(anyPathInFs: String?): Long {
            if (TextUtils.isEmpty(anyPathInFs)) return 0
            val statFs = StatFs(anyPathInFs)
            val blockSize: Long
            val totalSize: Long
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
                blockSize = statFs.blockSizeLong
                totalSize = statFs.blockCountLong
            } else {
                blockSize = statFs.blockSize.toLong()
                totalSize = statFs.blockCount.toLong()
            }
            return blockSize * totalSize
        }

        /**
         * Return the available size of file system.
         *
         * @param anyPathInFs Any path in file system.
         * @return the available size of file system
         */
        fun getFsAvailableSize(anyPathInFs: String?): Long {
            if (TextUtils.isEmpty(anyPathInFs)) return 0
            val statFs = StatFs(anyPathInFs)
            val blockSize: Long
            val availableSize: Long
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2) {
                blockSize = statFs.blockSizeLong
                availableSize = statFs.availableBlocksLong
            } else {
                blockSize = statFs.blockSize.toLong()
                availableSize = statFs.availableBlocks.toLong()
            }
            return blockSize * availableSize
        }
    }

}