/** A simple test harness to see what is happening with the gpg api
    development work I am doing */

#include <stdio.h>

/** A list of all the command line options supported and their possible single
    letter shortcuts. This is used to build the information for the ARG_PARSE
    command line parser. See the next block after this one... */
enum cmd_and_opt_values { aNull = 0,
    oArmor	  = 'a',
    aDetachedSign = 'b',
    aSym	  = 'c',
    aDecrypt	  = 'd',
    aEncr	  = 'e',
    oInteractive  = 'i',
    oKOption	  = 'k',
    oDryRun	  = 'n',
    oOutput	  = 'o',
    oQuiet	  = 'q',
    oRecipient	  = 'r',
    aSign	  = 's',
    oTextmodeShort= 't',
    oUser	  = 'u',
    oVerbose	  = 'v',
    oCompress	  = 'z',
    oNotation	  = 'N',
    oBatch	  = 500,
    aClearsign,
    aStore,
    aKeygen,
    aSignEncr,
    aSignKey,
    aLSignKey,
    aListPackets,
    aEditKey,
    aDeleteKey,
    aDeleteSecretKey,
    aKMode,
    aKModeC,
    aImport,
    aFastImport,
    aVerify,
    aVerifyFiles,
    aListKeys,
    aListSigs,
    aListSecretKeys,
    aSendKeys,
    aRecvKeys,
    aExport,
    aExportAll,
    aExportSecret,
    aExportSecretSub,
    aCheckKeys,
    aGenRevoke,
    aPrimegen,
    aPrintMD,
    aPrintMDs,
    aCheckTrustDB,
    aUpdateTrustDB,
    aFixTrustDB,
    aListTrustDB,
    aListTrustPath,
    aExportOwnerTrust,
    aImportOwnerTrust,
    aDeArmor,
    aEnArmor,
    aGenRandom,

    oTextmode,
    oFingerprint,
    oWithFingerprint,
    oAnswerYes,
    oAnswerNo,
    oKeyring,
    oSecretKeyring,
    oDefaultKey,
    oDefRecipient,
    oDefRecipientSelf,
    oNoDefRecipient,
    oOptions,
    oDebug,
    oDebugAll,
    oStatusFD,
    oNoComment,
    oNoVersion,
    oEmitVersion,
    oCompletesNeeded,
    oMarginalsNeeded,
    oMaxCertDepth,
    oLoadExtension,
    oRFC1991,
    oOpenPGP,
    oCipherAlgo,
    oDigestAlgo,
    oCompressAlgo,
    oPasswdFD,
    oCommandFD,
    oQuickRandom,
    oNoVerbose,
    oTrustDBName,
    oNoSecmemWarn,
    oNoArmor,
    oNoDefKeyring,
    oNoGreeting,
    oNoTTY,
    oNoOptions,
    oNoBatch,
    oHomedir,
    oWithColons,
    oWithKeyData,
    oSkipVerify,
    oCompressKeys,
    oCompressSigs,
    oAlwaysTrust,
    oEmuChecksumBug,
    oRunAsShmCP,
    oSetFilename,
    oSetPolicyURL,
    oUseEmbeddedFilename,
    oComment,
    oDefaultComment,
    oThrowKeyid,
    oForceV3Sigs,
    oForceMDC,
    oS2KMode,
    oS2KDigest,
    oS2KCipher,
    oCharset,
    oNotDashEscaped,
    oEscapeFrom,
    oLockOnce,
    oLockMultiple,
    oLockNever,
    oKeyServer,
    oEncryptTo,
    oNoEncryptTo,
    oLoggerFD,
    oUtf8Strings,
    oNoUtf8Strings,
    oDisableCipherAlgo,
    oDisablePubkeyAlgo,
    oAllowNonSelfsignedUID,
    oAllowFreeformUID,
    oNoLiteral,
    oSetFilesize,
    oHonorHttpProxy,
    oFastListMode,
    oListOnly,
    oIgnoreTimeConflict,
    oShowSessionKey,
    oOverrideSessionKey,
    oNoRandomSeedFile,
    oNoAutoKeyRetrieve,
    oMergeOnly,
    oTryAllSecrets,
    oTrustedKey,
    oEmu3DESS2KBug,  /* will be removed in 1.1 */
    oEmuMDEncodeBug,
aTest };

#include "api.h"

void init_api(void);

gpg_mtoken    apimagic;

int main(int argc, char *argv[]){
  enum cmd_and_opt_values cmd = 0;

  // Initialise the API
  init_api();

  // Make the magic token have the passphrase
  

  // Do the decryption
  set_cmd(&cmd, aDecrypt);
  init_keyring(cmd, 1, NULL, 1, NULL);
  decrypt_message("test.gpg");

  return 0;
}



