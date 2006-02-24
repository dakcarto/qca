/**
 * Copyright (C)  2004-2006  Brad Hards <bradh@frogmouth.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "hashunittest.h"
#include <qfile.h>

void HashUnitTest::initTestCase()
{
    m_init = new QCA::Initializer;
#include "../fixpaths.include"
}

void HashUnitTest::cleanupTestCase()
{
    QCA::unloadAllPlugins();
    delete m_init;
}

void HashUnitTest::md2test_data()
{
    // These are as specified in RFC 1319
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    QTest::newRow("md2()") << QByteArray("") << QString("8350e5a3e24c153df2275c9f80692773");
    QTest::newRow("md2(a)") << QByteArray("a") << QString("32ec01ec4a6dac72c0ab96fb34c0b5d1");
    QTest::newRow("md2(abc)") << QByteArray("abc")
			   << QString("da853b0d3f88d99b30283a69e6ded6bb");
    QTest::newRow("md2(messageDigest)") << QByteArray("message digest")
				     << QString("ab4f496bfb2a530b219ff33031fe06b0");
    QTest::newRow("md2([a-z])") << QByteArray("abcdefghijklmnopqrstuvwxyz")
			     << QString("4e8ddff3650292ab5a4108c3aa47940b");
    QTest::newRow("md2([A-z,0-9])") << QByteArray("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
				 << QString("da33def2a42df13975352846c30338cd");
    QTest::newRow("md2(nums)") << QByteArray("12345678901234567890123456789012345678901234567890123456789012345678901234567890")
			     << QString("d5976f79d83d3a0dc9806c3c66f3efd8");
}

void HashUnitTest::md2test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    // no MD2 support for libgcrypt...
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("md2", provider))
	    QSKIP(QString("MD2 not supported for "+provider).toLocal8Bit(), SkipSingle);
	QString hashResult = QCA::MD2(provider).hashToString(input);
	QCOMPARE( hashResult, expectedHash );
    }
}

void HashUnitTest::md4test_data()
{
    // These are as specified in RFC 1320
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    QTest::newRow("md4()") << QByteArray("") << QString("31d6cfe0d16ae931b73c59d7e0c089c0");
    QTest::newRow("md4(a)") << QByteArray("a") << QString("bde52cb31de33e46245e05fbdbd6fb24");
    QTest::newRow("md4(abc)") << QByteArray("abc")
			   << QString("a448017aaf21d8525fc10ae87aa6729d");
    QTest::newRow("md4(messageDigest)") << QByteArray("message digest")
				     << QString("d9130a8164549fe818874806e1c7014b");
    QTest::newRow("md4([a-z])") << QByteArray("abcdefghijklmnopqrstuvwxyz")
			     << QString("d79e1c308aa5bbcdeea8ed63df412da9");
    QTest::newRow("md4([A-z,0-9])") << QByteArray("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
				 << QString("043f8582f241db351ce627e153e7f0e4");
    QTest::newRow("md4(nums)") << QByteArray("12345678901234567890123456789012345678901234567890123456789012345678901234567890")
			    << QString("e33b4ddc9c38f2199c3e7b164fcc0536");
}


void HashUnitTest::md4test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    // No MD4 support for NSS?
    // providersToTest.append("qca-nss");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("md4", provider))
	    QWARN(QString("MD4 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::MD4(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}

void HashUnitTest::md5test_data()
{
    // These are as specified in RFC 1321
    // They also match Australian Standard (AS) 2805.1.3.2-2000 Appendix A
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    QTest::newRow("md5()") << QByteArray("") << QString("d41d8cd98f00b204e9800998ecf8427e");
    QTest::newRow("md5(a)") << QByteArray("a") << QString("0cc175b9c0f1b6a831c399e269772661");
    QTest::newRow("md5(abc)") << QByteArray("abc")
			   << QString("900150983cd24fb0d6963f7d28e17f72");
    QTest::newRow("md5(messageDigest)") << QByteArray("message digest")
				     << QString("f96b697d7cb7938d525a2f31aaf161d0");
    QTest::newRow("md5([a-z])") << QByteArray("abcdefghijklmnopqrstuvwxyz")
			     << QString("c3fcd3d76192e4007dfb496cca67e13b");
    QTest::newRow("md5([A-z,0-9])") << QByteArray("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
				 << QString("d174ab98d277d9f5a5611c2c9f419d9f");
    QTest::newRow("md5(nums)") << QByteArray("12345678901234567890123456789012345678901234567890123456789012345678901234567890")
					<< QString("57edf4a22be3c955ac49da2e2107b67a");
}

void HashUnitTest::md5test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    providersToTest.append("default");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("md5", provider))
	    QWARN(QString("MD5 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::MD5(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}


void HashUnitTest::md5filetest()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    providersToTest.append("default");
    
    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("md5", provider))
	    QWARN(QString("MD5 not supported for "+provider).toLocal8Bit());
	else {
	    QFile f1( "./data/empty" );
	    if ( f1.open( QIODevice::ReadOnly ) ) {
		QCA::MD5 hashObj(provider);
		hashObj.update( f1 );
		QCOMPARE( QString( QCA::arrayToHex( hashObj.final() ) ),
			 QString( "d41d8cd98f00b204e9800998ecf8427e" ) );
	    } else {
		QWARN( "./data/empty could not be opened - do you need to create it?");
	    }

	    QFile f2( "./data/Botan-1.4.1.tar.bz2" );
	    if ( f2.open( QIODevice::ReadOnly ) ) {
		QCA::MD5 hashObj(provider);
		hashObj.update( f2 );
		QCOMPARE( QString( QCA::arrayToHex( hashObj.final() ) ),
			 QString( "7c4b3d8a360c6c3cb647160fa9adfe71" ) );
	    } else {
		QWARN( "./data/Botan-1.4.1.tar.bz2 could not be opened - do you need to download it?");
	    }
	    

	    QFile f3( "./data/linux-2.6.7.tar.bz2" );
	    if ( f3.open( QIODevice::ReadOnly ) ) {
		QCA::MD5 hashObj(provider);
		hashObj.update( f3 );
		QCOMPARE( QString( QCA::arrayToHex( hashObj.final() ) ),
			 QString( "a74671ea68b0e3c609e8785ed8497c14" ) );
	    } else {
		QWARN( "./data/linux-2.6.7.tar.bz2 could not be opened - do you need to download it?");
	    }
	    
	    QFile f4( "./data/scribus-1.2.tar.bz2" );
	    if ( f4.open( QIODevice::ReadOnly ) ) {
		QCA::MD5 hashObj(provider);
		hashObj.update( f4 );
		QCOMPARE( QString( QCA::arrayToHex( hashObj.final() ) ),
			 QString( "7d2c2b228f9a6ff82c9401fd54bdbe16" ) );
	    } else {
		QWARN( "./data/scribus-1.2.tar.bz2 could not be opened - do you need to download it?");
	    }
	}
    }
}

void HashUnitTest::sha0test_data()
{
    // These are extracted from OpenOffice.org 1.1.2, in sal/workben/t_digest.c
    // Check FIPS 180-1?
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    QTest::newRow("sha0(abc)") << QByteArray("abc") << QString("0164b8a914cd2a5e74c4f7ff082c4d97f1edf880");
    QTest::newRow("sha0(abc)") << QByteArray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")
			    << QString("d2516ee1acfa5baf33dfc1c471e438449ef134c8");
}

void HashUnitTest::sha0test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    // No SHA0 for botan, gcrypt or nss
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha0", provider))
	    QWARN(QString("SHA0 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::SHA0(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}

void HashUnitTest::sha0longtest()
{
    QByteArray fillerString;
    fillerString.fill('a', 1000);
    
    // This test extracted from OpenOffice.org 1.1.2, in sal/workben/t_digest.c

    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    // No SHA0 for botan, gcrypt or nss
    
    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha0", provider))
	    QWARN(QString("SHA0 not supported for "+provider).toLocal8Bit());
	else {
	    QCA::SHA0 shaHash(provider);
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("3232affa48628a26653b5aaa44541fd90d690603" ) );
	    
	    shaHash.clear();
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("3232affa48628a26653b5aaa44541fd90d690603" ) );
	}
    }
}

void HashUnitTest::sha1test_data()
{
    // These are as specfied in FIPS 180-2. Matches RFC3174
    // Some additions from Australian Standard (AS) 2805.13.3-2000
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    // FIPS 180-2, Appendix A.1
    QTest::newRow("sha1(abc)") << QByteArray("abc") << QString("a9993e364706816aba3e25717850c26c9cd0d89d");

    // FIPS 180-2, Appendix A.2
    QTest::newRow("sha1(a-q)") << QByteArray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")
			    << QString("84983e441c3bd26ebaae4aa1f95129e5e54670f1");

    // AS 2805.13.3-200 Appendix A
    // also has some duplicates from FIPS 180-2
    QTest::newRow("sha1()") << QByteArray("") << QString("da39a3ee5e6b4b0d3255bfef95601890afd80709");
    QTest::newRow("sha1(a)") << QByteArray("a") << QString("86f7e437faa5a7fce15d1ddcb9eaeaea377667b8");
    QTest::newRow("sha1(a-z)") << QByteArray("abcdefghijklmnopqrstuvwxyz")
			    << QString("32d10c7b8cf96570ca04ce37f2a19d84240d3a89");
}

void HashUnitTest::sha1test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    providersToTest.append("default");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha1", provider))
	    QWARN(QString("SHA1 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::SHA1(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}

void HashUnitTest::sha1longtest()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    providersToTest.append("default");
    
    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha1", provider))
	    QWARN(QString("SHA1 not supported for "+provider).toLocal8Bit());
	else {
	    // QTime t;
	    // t.start();
	    QByteArray fillerString;
	    fillerString.fill('a', 1000);
	    
	    // This test extracted from OpenOffice.org 1.1.2, in sal/workben/t_digest.c
	    // It basically reflects FIPS 180-2, Appendix A.3
	    // Also as per AS 2805.13.3-2000 Appendix A
	    QCA::SHA1 shaHash(provider);
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("34aa973cd4c4daa4f61eeb2bdbad27316534016f") );

	    QFile f1( "./data/empty" );
	    if ( f1.open( QIODevice::ReadOnly ) ) {
		QCA::SHA1 hashObj(provider);
		hashObj.update( f1 );
		QCOMPARE( QString( QCA::arrayToHex( hashObj.final() ) ),
			 QString( "da39a3ee5e6b4b0d3255bfef95601890afd80709" ) );
	    } else {
		QWARN( "./data/empty could not be opened - do you need to create it?");
	    }
	    
	    QFile f2( "./data/Botan-1.4.1.tar.bz2" );
	    if ( f2.open( QIODevice::ReadOnly ) ) {
		QCA::SHA1 hashObj(provider);
		hashObj.update( f2 );
		QCOMPARE( QString( QCA::arrayToHex( hashObj.final() ) ),
			 QString( "cda343591428a68e22bd2e349b890cbafb642cf7" ) );
	    } else {
		QWARN( "./data/Botan-1.4.1.tar.bz2 could not be opened - do you need to download it?");
	    }

	    QFile f3( "./data/linux-2.6.7.tar.bz2" );
	    if ( f3.open( QIODevice::ReadOnly ) ) {
		QCA::SHA1 hashObj(provider);
		hashObj.update( f3 );
		QCOMPARE( QString( QCA::arrayToHex( hashObj.final() ) ),
			 QString( "a030a9c6dcd10c5d90a86f915ad4710084cbca71" ) );
	    } else {
		QWARN( "./data/linux-2.6.7.tar.bz2 could not be opened - do you need to download it?");
	    }
	    
	    QFile f4( "./data/scribus-1.2.tar.bz2" );
	    if ( f4.open( QIODevice::ReadOnly ) ) {
		QCA::SHA1 hashObj(provider);
		hashObj.update( f4 );
		QCOMPARE( QString( QCA::arrayToHex( hashObj.final() ) ),
			 QString( "a1fb6ed6acfd92381055b310d926d6e83e76ff1e" ) );
	    } else {
		QWARN( "./data/scribus-1.2.tar.bz2 could not be opened - do you need to download it?");
	    }
	    // qDebug() << "SHA1: " << provider << " elapsed " << t.elapsed();
	}
    }
}

void HashUnitTest::sha224test_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    // These are as specfied in FIPS 180-2, change notice 1

    // FIPS 180-2, Appendix B.1
    QTest::newRow("sha224(abc)") << QByteArray("abc") << QString("23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7");

    // FIPS 180-2, Appendix B.2
    QTest::newRow("sha224(aq)") << QByteArray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")
			      << QString("75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525");
}

void HashUnitTest::sha224test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-ipp");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha224", provider))
	    QWARN(QString("SHA224 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::SHA224(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}


void HashUnitTest::sha224longtest()
{
    QByteArray fillerString;
    fillerString.fill('a', 1000);
    
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-ipp");

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha224", provider))
	    QWARN(QString("SHA224 not supported for "+provider).toLocal8Bit());
	else {
	    QCA::SHA224 shaHash(provider);

	    // This basically reflects FIPS 180-2, change notice 1, section 3
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67") );
	    
	    shaHash.clear();
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67") );
	}
    }
}

void HashUnitTest::sha256test_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    // These are as specfied in FIPS 180-2

    // FIPS 180-2, Appendix B.1
    QTest::newRow("sha256(abc)") << QByteArray("abc") << QString("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

    // FIPS 180-2, Appendix B.2
    QTest::newRow("sha256(abc)") << QByteArray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")
			      << QString("248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
}

void HashUnitTest::sha256test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha256", provider))
	    QWARN(QString("SHA256 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::SHA256(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}

void HashUnitTest::sha256longtest()
{
    QByteArray fillerString;
    fillerString.fill('a', 1000);
    
    QStringList providersToTest;
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    
    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha256", provider))
	    QWARN(QString("SHA256 not supported for "+provider).toLocal8Bit());
	else {
	    QCA::SHA256 shaHash(provider);

	    // This basically reflects FIPS 180-2, change notice 1, section 3
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0") );

	    
	    shaHash.clear();
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0") );
	}
    }
}


void HashUnitTest::sha384test_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    // These are as specfied in FIPS 180-2, and from Aaron Gifford's SHA2 tests

    // FIPS 180-2, Appendix B.1
    QTest::newRow("sha384(abc)") << QByteArray("abc") 
			      << QString("cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7");

    // FIPS 180-2, Appendix B.2
    QTest::newRow("sha384(a-u)") << QByteArray("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu")
			      << QString("09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039");

    // Aaron Gifford, vector002.info
    QTest::newRow("sha384(a-q)") << QByteArray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")
			      << QString("3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b");


}

void HashUnitTest::sha384test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha384", provider))
	    QWARN(QString("SHA384 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::SHA384(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}

void HashUnitTest::sha384longtest()
{
    QByteArray fillerString;
    fillerString.fill('a', 1000);
    
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    
    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha384", provider))
	    QWARN(QString("SHA384 not supported for "+provider).toLocal8Bit());
	else {
	    // QTime t;
	    // t.start();
	    QCA::SHA384 shaHash(provider);

	    // This basically reflects FIPS 180-2, change notice 1, section 3
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("9d0e1809716474cb086e834e310a4a1ced149e9c00f248527972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985") );

	    
	    shaHash.clear();
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("9d0e1809716474cb086e834e310a4a1ced149e9c00f248527972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985") );
	    // qDebug() << "SHA384: " << provider << " elapsed " << t.elapsed();
	}
    }
}


// These are as specfied in FIPS 180-2, and from Aaron Gifford's SHA2 tests
void HashUnitTest::sha512test_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    // FIPS 180-2, Appendix C.1
    QTest::newRow("sha512(abc)") << QByteArray("abc")
			      << QString("ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f");
    // FIPS 180-2, Appendix C.2
    QTest::newRow("sha512(a-u)") << QByteArray("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu")
			      << QString("8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909");

    // Aaron Gifford, vector002.info
    QTest::newRow("sha512(a-q)") << QByteArray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")
			      << QString("204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445");
}

void HashUnitTest::sha512test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha512", provider))
	    QWARN(QString("SHA512 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::SHA512(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}

void HashUnitTest::sha512longtest()
{
    QByteArray fillerString;
    fillerString.fill('a', 1000);
    
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    providersToTest.append("qca-nss");
    providersToTest.append("qca-ipp");
    
    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("sha512", provider))
	    QWARN(QString("SHA512 not supported for "+provider).toLocal8Bit());
	else {
	    QCA::SHA512 shaHash(provider);

	    // This basically reflects FIPS 180-2, change notice 1, section 3
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b") );

	    shaHash.clear();
	    for (int i=0; i<1000; i++)
		shaHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(shaHash.final())),
		     QString("e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b") );
	}
    }
}

// These are as specified in http://www.esat.kuleuven.ac.be/~bosselae/ripemd160.html
// ISO/IEC 10118-3 costs a bit of money.
void HashUnitTest::rmd160test_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedHash");

    QTest::newRow("rmd160()") << QByteArray("") << QString("9c1185a5c5e9fc54612808977ee8f548b2258d31");
    QTest::newRow("rmd160(a)") << QByteArray("a") << QString("0bdc9d2d256b3ee9daae347be6f4dc835a467ffe");
    QTest::newRow("rmd160(abc)") << QByteArray("abc") << QString("8eb208f7e05d987a9b044a8e98c6b087f15a0bfc");
    QTest::newRow("rmd160(md)") << QByteArray("message digest") << QString("5d0689ef49d2fae572b881b123a85ffa21595f36");
    QTest::newRow("rmd160(a-z)") << QByteArray("abcdefghijklmnopqrstuvwxyz") << QString("f71c27109c692c1b56bbdceb5b9d2865b3708dbc");
    QTest::newRow("rmd160(a-q)") << QByteArray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")
			      << QString("12a053384a9c0c88e405a06c27dcf49ada62eb2b");
    QTest::newRow("rmd160(A-9)") << QByteArray("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
			      << QString("b0e20b6e3116640286ed3a87a5713079b21f5189");
    QTest::newRow("rmd160(1-0)") << QByteArray("12345678901234567890123456789012345678901234567890123456789012345678901234567890")
			      << QString("9b752e45573d4b39f4dbd3323cab82bf63326bfb");
};


void HashUnitTest::rmd160test()
{
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedHash);

    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("ripemd160", provider))
	    QWARN(QString("RIPEMD160 not supported for "+provider).toLocal8Bit());
	else {
	    QString hashResult = QCA::RIPEMD160(provider).hashToString(input);
	    QCOMPARE( hashResult, expectedHash );
	}
    }
}

void HashUnitTest::rmd160longtest()
{
    QByteArray fillerString;
    fillerString.fill('a', 1000);
    
    QStringList providersToTest;
    providersToTest.append("qca-openssl");
    providersToTest.append("qca-gcrypt");
    providersToTest.append("qca-botan");
    
    foreach(QString provider, providersToTest) {
	if(!QCA::isSupported("ripemd160", provider))
	    QWARN(QString("RIPEMD160 not supported for "+provider).toLocal8Bit());
	else {
	    QCA::RIPEMD160 rmdHash(provider);

	    // This is the "million times 'a' test"
	    for (int i=0; i<1000; i++)
	        rmdHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(rmdHash.final())),
		     QString("52783243c1697bdbe16d37f97f68f08325dc1528") );
	    
	    rmdHash.clear();
	    for (int i=0; i<1000; i++)
		rmdHash.update(fillerString);
	    QCOMPARE( QString(QCA::arrayToHex(rmdHash.final())),
		     QString("52783243c1697bdbe16d37f97f68f08325dc1528") );

	    // This is the "8 rounds of 1234567890" test.
	    // It also ensure that we can re-use hash objects correctly.
	    static char bindata[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30 };
	    QByteArray fillerArray( bindata, sizeof(bindata) ); // "1234567890"
	    rmdHash.clear();
	    for (int i=0; i<8; i++)
		rmdHash.update(fillerArray);
	    QCOMPARE( QString(QCA::arrayToHex(rmdHash.final())),
		     QString("9b752e45573d4b39f4dbd3323cab82bf63326bfb") );

	}
    }
}


QTEST_MAIN(HashUnitTest)
