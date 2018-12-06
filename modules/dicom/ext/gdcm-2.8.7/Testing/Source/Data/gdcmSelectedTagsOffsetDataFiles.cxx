/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library

  Copyright (c) 2006-2011 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http:/gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/


/*
 * See TestReaderSelectedTags
 */

#if 0
struct StreamOffset
{
  const char *filename;
  std::streamoff offset;
};
#endif

static const StreamOffset gdcmSelectedTagsOffsetDataFiles[] = {
{ "SC16BitsAllocated_8BitsStoredJ2K.dcm", 752},
{ "SC16BitsAllocated_8BitsStoredJPEG.dcm", 946},
{ "CT-MONO2-12-lomb-an2.acr",1222 },
{ "LIBIDO-8-ACR_NEMA-Lena_128_128.acr",174 },
{ "MR-MONO2-12-an2.acr",1858 },
{ "SIEMENS_SOMATOM-12-ACR_NEMA-ZeroLengthUs.acr",5056 },
{ "gdcm-ACR-LibIDO.acr",174 },
{ "gdcm-MR-SIEMENS-16-2.acr",6044 },
{ "libido1.0-vol.acr",162 },
{ "test.acr",174 },
{ "00191113.dcm",922 },
{ "012345.002.050.dcm",9592 },
{ "05115014-mr-siemens-avanto-syngo-with-palette-icone.dcm",41936 },
{ "05148044-mr-siemens-avanto-syngo.dcm",55872 },
{ "3E768EB7.dcm",3222 },
{ "ACUSON-24-YBR_FULL-RLE-b.dcm",2020 },
{ "ACUSON-24-YBR_FULL-RLE.dcm",1908 },
{ "ALOKA_SSD-8-MONO2-RLE-SQ.dcm",1386 },
{ "AMIInvalidPrivateDefinedLengthSQasUN.dcm",12732 },
{ "BugGDCM2_UndefItemWrongVL.dcm",9562 },
{ "CR-MONO1-10-chest.dcm",768 },
{ "CT-MONO2-16-ankle.dcm",1140 },
{ "CT-MONO2-16-brain.dcm",1668 },
{ "CT-MONO2-16-chest.dcm",1626 },
{ "CT-MONO2-16-ort.dcm",1666 },
{ "CT-MONO2-8-abdo.dcm",788 },
{ "CT-SIEMENS-Icone-With-PaletteColor.dcm",8150 },
{ "CT-SIEMENS-MissingPixelDataInIconSQ.dcm",20496 },
{ "CT_16b_signed-UsedBits13.dcm",2574 },
{ "DCMTK_JPEGExt_12Bits.dcm",784 },
{ "DMCPACS_ExplicitImplicit_BogusIOP.dcm",20018 },
{ "DX_GE_FALCON_SNOWY-VOI.dcm",104002 },
{ "DX_J2K_0Padding.dcm",1482 },
{ "D_CLUNIE_CT1_J2KI.dcm",6584 },
{ "D_CLUNIE_CT1_J2KR.dcm",6484 },
{ "D_CLUNIE_CT1_JLSL.dcm",6452 },
{ "D_CLUNIE_CT1_JLSN.dcm",6566 },
{ "D_CLUNIE_CT1_JPLL.dcm",6456 },
{ "D_CLUNIE_CT1_RLE.dcm",6390 },
{ "D_CLUNIE_CT2_JPLL.dcm",1744 },
{ "D_CLUNIE_CT2_RLE.dcm",1676 },
{ "D_CLUNIE_MR1_JPLL.dcm",1750 },
{ "D_CLUNIE_MR1_JPLY.dcm",1836 },
{ "D_CLUNIE_MR1_RLE.dcm",1682 },
{ "D_CLUNIE_MR2_JPLL.dcm",1946 },
{ "D_CLUNIE_MR2_JPLY.dcm",2034 },
{ "D_CLUNIE_MR2_RLE.dcm",1878 },
{ "D_CLUNIE_MR3_JPLL.dcm",9570 },
{ "D_CLUNIE_MR3_JPLY.dcm",9658 },
{ "D_CLUNIE_MR3_RLE.dcm",9504 },
{ "D_CLUNIE_MR4_JPLL.dcm",1968 },
{ "D_CLUNIE_MR4_JPLY.dcm",2056 },
{ "D_CLUNIE_MR4_RLE.dcm",1900 },
{ "D_CLUNIE_NM1_JPLL.dcm",2890 },
{ "D_CLUNIE_NM1_JPLY.dcm",2978 },
{ "D_CLUNIE_NM1_RLE.dcm",2826 },
{ "D_CLUNIE_RG1_JPLL.dcm",1858 },
{ "D_CLUNIE_RG1_RLE.dcm",1794 },
{ "D_CLUNIE_RG2_JPLL.dcm",1444 },
{ "D_CLUNIE_RG2_JPLY.dcm",1532 },
{ "D_CLUNIE_RG2_RLE.dcm",1368 },
{ "D_CLUNIE_RG3_JPLL.dcm",1514 },
{ "D_CLUNIE_RG3_JPLY.dcm",1602 },
{ "D_CLUNIE_RG3_RLE.dcm",1432 },
{ "D_CLUNIE_SC1_JPLY.dcm",1340 },
{ "D_CLUNIE_SC1_RLE.dcm",1176 },
{ "D_CLUNIE_US1_RLE.dcm",1306 },
{ "D_CLUNIE_VL1_RLE.dcm",1188 },
{ "D_CLUNIE_VL2_RLE.dcm",1186 },
{ "D_CLUNIE_VL3_RLE.dcm",1198 },
{ "D_CLUNIE_VL4_RLE.dcm",1180 },
{ "D_CLUNIE_VL6_RLE.dcm",1198 },
{ "D_CLUNIE_XA1_JPLL.dcm",1192 },
{ "D_CLUNIE_XA1_JPLY.dcm",1280 },
{ "D_CLUNIE_XA1_RLE.dcm",1116 },
{ "DermaColorLossLess.dcm",928 },
{ "ELSCINT1_JP2vsJ2K.dcm",1808 },
{ "ELSCINT1_PMSCT_RLE1.dcm",67912 },
{ "ExplicitVRforPublicElementsImplicitVRforShadowElements.dcm",9534 },
{ "FUJI-10-MONO1-ACR_NEMA_2.dcm",854 },
{ "GDCMJ2K_TextGBR.dcm",758 },
{ "GE_CT_With_Private_compressed-icon.dcm",10798 },
{ "GE_DLX-8-MONO2-Multiframe-Jpeg_Lossless.dcm",14126 },
{ "GE_DLX-8-MONO2-Multiframe.dcm",4976 },
{ "GE_DLX-8-MONO2-PrivateSyntax.dcm",2912 },
{ "GE_GENESIS-16-MONO2-Uncompressed-UnusualVR.dcm",9654 },
{ "GE_GENESIS-16-MONO2-WrongLengthItem.dcm",1922 },
{ "GE_LOGIQBook-8-RGB-HugePreview.dcm",935596 },
{ "GE_MR_0025xx1bProtocolDataBlock.dcm",11870 },
{ "GE_RHAPSODE-16-MONO2-JPEG-Fragments.dcm",6456 },
{ "IM-0001-0066.CommandTag00.dcm",3042 },
{ "ITK_GDCM124_MultiframeSecondaryCaptureInvalid.dcm",1132 },
{ "JDDICOM_Sample2-dcmdjpeg.dcm",836 },
{ "JDDICOM_Sample2.dcm",822 },
{ "JPEGDefinedLengthSequenceOfFragments.dcm",2862 },
{ "JPEG_LossyYBR.dcm",2366 },
{ "KODAK-12-MONO1-Odd_Terminated_Sequence.dcm",9126 },
{ "KODAK_CompressedIcon.dcm",8634 },
{ "LEADTOOLS_FLOWERS-16-MONO2-JpegLossless.dcm",1932 },
{ "LEADTOOLS_FLOWERS-16-MONO2-RLE.dcm",1930 },
{ "LEADTOOLS_FLOWERS-16-MONO2-Uncompressed.dcm",1930 },
{ "LEADTOOLS_FLOWERS-24-RGB-JpegLossless.dcm",1924 },
{ "LEADTOOLS_FLOWERS-24-RGB-JpegLossy.dcm",1924 },
{ "LEADTOOLS_FLOWERS-24-RGB-Uncompressed.dcm",1922 },
{ "LEADTOOLS_FLOWERS-8-MONO2-JpegLossy.dcm",1932 },
{ "LEADTOOLS_FLOWERS-8-MONO2-RLE.dcm",1930 },
{ "LEADTOOLS_FLOWERS-8-MONO2-Uncompressed.dcm",1930 },
{ "LEADTOOLS_FLOWERS-8-PAL-RLE.dcm",3526 },
{ "LEADTOOLS_FLOWERS-8-PAL-Uncompressed.dcm",3526 },
{ "LIBIDO-24-ACR_NEMA-Rectangle.dcm",174 },
{ "LJPEG_BuginGDCM12.dcm",2236 },
{ "MARCONI_MxTWin-12-MONO2-JpegLossless-ZeroLengthSQ.dcm",1904 },
{ "MAROTECH_CT_JP2Lossy.dcm",33844 },
{ "MEDILABInvalidCP246_EVRLESQasUN.dcm",19162 },
{ "MR-Brucker-CineTagging-NonSquarePixels.dcm",1530 },
{ "MR-MONO2-12-shoulder.dcm",1568 },
{ "MR-MONO2-16-head.dcm",1796 },
{ "MR-MONO2-8-16x-heart.dcm",908 },
{ "MR-SIEMENS-DICOM-WithOverlays-extracted-overlays.dcm",42600 },
{ "MR-SIEMENS-DICOM-WithOverlays.dcm",42404 },
{ "MR16BitsAllocated_8BitsStored.dcm",928 },
{ "MR_ELSCINT1_00e1_1042_SQ_feff_00e0_Item.dcm",7696 },
{ "MR_GE_with_Private_Compressed_Icon_0009_1110.dcm",20700 },
{ "MR_Philips-Intera_BreaksNOSHADOW.dcm",7618 },
{ "MR_Philips_Intera_No_PrivateSequenceImplicitVR.dcm",9500 },
{ "MR_Philips_Intera_PrivateSequenceExplicitVR_in_SQ_2001_e05f_item_wrong_lgt_use_NOSHADOWSEQ.dcm",7512 },
{ "MR_Philips_Intera_PrivateSequenceImplicitVR.dcm",7018 },
{ "MR_Philips_Intera_SwitchIndianess_noLgtSQItem_in_trueLgtSeq.dcm",9264 },
{ "MR_SIEMENS_forceLoad29-1010_29-1020.dcm",60556 },
{ "MR_Spectroscopy_SIEMENS_OF.dcm",2102344 },
{ "NM-MONO2-16-13x-heart.dcm",1222 },
{ "NM-PAL-16-PixRep1.dcm",4432 },
{ "NM_Kakadu44_SOTmarkerincons.dcm",1564 },
{ "OT-MONO2-8-a7.dcm",430 },
{ "OT-PAL-8-face.dcm",1646 },
{ "OsirixFake16BitsStoredFakeSpacing.dcm",1306 },
{ "PHILIPS_Brilliance_ExtraBytesInOverlay.dcm",34538 },
{ "PHILIPS_GDCM12xBug.dcm",9330 },
{ "PHILIPS_GDCM12xBug2.dcm",9662 },
{ "PHILIPS_Gyroscan-12-Jpeg_Extended_Process_2_4.dcm",5944 },
{ "PHILIPS_Gyroscan-12-MONO2-Jpeg_Lossless.dcm",15366 },
{ "PHILIPS_Gyroscan-8-MONO2-Odd_Sequence.dcm",6576 },
{ "PHILIPS_Intera-16-MONO2-Uncompress.dcm",9264 },
{ "PICKER-16-MONO2-Nested_icon.dcm",18220 },
{ "PICKER-16-MONO2-No_DicomV3_Preamble.dcm",1398 },
{ "PhilipsInteraSeqTermInvLen.dcm",7440 },
{ "PrivateGEImplicitVRBigEndianTransferSyntax16Bits.dcm",14964 },
{ "RadBWLossLess.dcm",930 },
{ "SIEMENS-12-Jpeg_Process_2_4-Lossy-a.dcm",3032 },
{ "SIEMENS-MR-RGB-16Bits.dcm",49840 },
{ "SIEMENS_CSA2.dcm",104340 },
{ "SIEMENS_ImageLocationUN.dcm",6336 },
{ "SIEMENS_MAGNETOM-12-ACR_NEMA_2-Modern.dcm",6044 },
{ "SIEMENS_MAGNETOM-12-MONO2-FileSeq0.dcm",6422 },
{ "SIEMENS_MAGNETOM-12-MONO2-FileSeq1.dcm",6422 },
{ "SIEMENS_MAGNETOM-12-MONO2-FileSeq2.dcm",6422 },
{ "SIEMENS_MAGNETOM-12-MONO2-FileSeq3.dcm",6422 },
{ "SIEMENS_MAGNETOM-12-MONO2-GDCM12-VRUN.dcm",5158 },
{ "SIEMENS_MAGNETOM-12-MONO2-Uncompressed.dcm",5148 },
{ "SIEMENS_MAGNETOM-12-MONO2-VRUN.dcm",5290 },
{ "SIEMENS_MOSAIC_12BitsStored-16BitsJPEG.dcm",105714 },
{ "SIEMENS_Sonata-12-MONO2-SQ.dcm",51792 },
{ "SIEMENS_Sonata-16-MONO2-Value_Multiplicity.dcm",25674 },
{ "SignedShortLosslessBug.dcm",1772 },
{ "TG18-CH-2k-01.dcm",2300 },
{ "THERALYS-12-MONO2-Uncompressed-Even_Length_Tag.dcm",1831 },
{ "TOSHIBA_J2K_OpenJPEGv2Regression.dcm",2350 },
{ "TOSHIBA_J2K_SIZ0_PixRep1.dcm",2334 },
{ "TOSHIBA_J2K_SIZ1_PixRep0.dcm",2350 },
{ "TOSHIBA_MRT150-16-MONO2-ACR_NEMA_2.dcm",1122 },
{ "TheralysGDCM120Bug.dcm",8438 },
{ "US-GE-4AICL142.dcm",14060 },
{ "US-IRAD-NoPreambleStartWith0003.dcm",3322 },
{ "US-IRAD-NoPreambleStartWith0005.dcm",2482 },
{ "US-MONO2-8-8x-execho.dcm",964 },
{ "US-PAL-8-10x-echo.dcm",2416 },
{ "US-RGB-8-epicard.dcm",1000 },
{ "US-RGB-8-esopecho.dcm",892 },
{ "UnexpectedSequenceDelimiterInFixedLengthSequence.dcm",4178 },
{ "XA-MONO2-8-12x-catheter.dcm",994 },
{ "fffc0000UN.dcm",1142 },
{ "gdcm-CR-DCMTK-16-NonSamplePerPix.dcm",728 },
{ "gdcm-JPEG-Extended.dcm",2934 },
{ "gdcm-JPEG-LossLess3a.dcm",2528 },
{ "gdcm-JPEG-LossLessThoravision.dcm",3132 },
{ "gdcm-MR-PHILIPS-16-Multi-Seq.dcm",35834 },
{ "gdcm-MR-PHILIPS-16-NonSquarePixels.dcm",4998 },
{ "gdcm-US-ALOKA-16.dcm",258732 },
{ "rle16loo.dcm",394214 },
{ "rle16sti.dcm",394158 },
{ "simpleImageWithIcon.dcm",17466 },
{ "undefined_length_un_vr.dcm",9156 },
{ "JPEGInvalidSecondFrag.dcm",4788 },
{ "lena512_rot90.j2k.dcm",756 },
{ "ELSCINT1_PMSCT_RLE1_priv.dcm",298002 },
{ "00191113.dcm",922 },
{ "SIEMENS_SYNGO-12-MONO2-LUTWrongLength512.dcm", 48714 },
{ "JPEGNote_empty.dcm", 704 },
{ "JPEGLS_CharLS_10742.dcm", 748 },
{ "JPEGLosslessYBR_FULL_422.dcm", 802 },
{ "JPEGNote_missing.dcm", 596 },
{ "JPEGNote_bogus.dcm", 722 },
{ "RLEDebianBug816607Orig.dcm", 768 },
{ "IllegalGroup2ImplicitTS.dcm", 2264 },
{ "JPEG_LS_InvalidEscapeSequence_COM_padding.dcm", 752 },
{ "LengthOfItemLarger.dcm", 12748 },
{ "GE_MR_0025xx1bProtocolDataBlockXML.dcm",6462 },
{ "dicomdir_Acusson_WithPrivate_WithSR",3826 },
{ "dicomdir_Pms_WithVisit_WithPrivate_WithStudyComponents",37218 },
{ "dicomdir_Pms_With_heavy_embedded_sequence",3003414 },
{ "dicomdir_With_embedded_icons",7125212 },
{ "DICOMDIR",1112434 },
{ "DICOMDIR-Philips-EasyVision-4200-Entries",1297194 },
{ "DICOMDIR_MR_B_VA12A",14853880 },

/* Stopping condition */
{ 0 ,0 },
};
