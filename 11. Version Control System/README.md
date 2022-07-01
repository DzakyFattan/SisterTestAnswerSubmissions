# Version Control System Sederhana

#### Oleh 13520003 - Dzaky Fattan Rizqullah

Version Control System (VCS) sederhana yang ditulis dalam Bahasa Python.

Telah dicoba dan dapat dijalankan pada Windows 11.

VCS ini menggunakan teknik penyimpanan versi 'snapshot' karena implementasinya yang sangat sederhana, meskipun akan memakan lebih banyak penyimpanan.

### Cara penggunaan:
0. Pastikan Python (dicoba pada versi 3.10.4 64 bit) telah ter-install. Pindahkan folder `.vcs` ke direktori yang nantinya akan dibuat folder repositori.
1. `.vcs/init <directory>` untuk membuat (init) repositori baru bernama `<directory>`.
2. `cd` menuju folder repositori. Perubahan terhadap repositori mulai dapat dilakukan.
3. `.vcs/commit <message>` untuk melakukan commit setelah melakukan perubahan.
4. `.vcs/log` untuk melihat riwayat commit.
5. `.vcs/tag <head> <tag>` untuk menambahkan tag bernama <tag> untuk commit <head>. Tidak terlalu bermanfaat.
6. `.vcs/head` untuk mengganti isi repositori ke versi yang sesuai.

### Perintah yang tersedia:
1. `.vcs/init <directory>` untuk membuat (init) repositori baru bernama `<directory>`.
2. `.vcs/commit <message>` untuk melakukan commit setelah melakukan perubahan.
3. `.vcs/log` untuk melihat riwayat commit.
4. `.vcs/head <versionid>` untuk mengganti isi repositori ke versi yang sesuai.
5. (Tambahan) `.vcs/tag <versionid> <tag>` untuk menambahkan tag bernama <tag> untuk commit <versionid>. Tidak ada fungsi tambahan lagi.