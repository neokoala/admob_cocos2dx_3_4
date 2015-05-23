/****************************************************************************
Copyright (c) 2012-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.plugin;

import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Set;

import com.google.android.gms.ads.*;
import com.google.android.gms.ads.AdRequest;

import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.view.WindowManager;

public class AdsAdmob implements InterfaceAds {

	private static final String LOG_TAG = "AdsAdmob";
	private static Activity mContext = null;
	private static boolean bDebug = false;
	private static AdsAdmob mAdapter = null;

	private AdView adView = null;
	private String mPublishID = "";
	private Set<String> mTestDevices = null;
	private WindowManager mWm = null;
    
	private static final int kSizeBanner = 1;
	private static final int kSizeFullBanner = 2;
	private static final int kSizeLargeBanner = 3;
	private static final int kSizeLeaderboard = 4;
	private static final int kSizeMediumRectangle = 5;
    private static final int kSizeSmartBanner = 6;
    private static final int kSizeSkyscraper = 7;

	private static final int ADMOB_TYPE_BANNER = 1;
	private static final int ADMOB_TYPE_FULLSCREEN = 2;

	protected static void LogE(String msg, Exception e) {
		Log.e(LOG_TAG, msg, e);
		e.printStackTrace();
	}

	protected static void LogD(String msg) {
		if (bDebug) {
			Log.d(LOG_TAG, msg);
		}
	}

	public AdsAdmob(Context context) {
		mContext = (Activity) context;
		mAdapter = this;
	}

	@Override
	public void setDebugMode(boolean debug) {
		bDebug = debug;
	}

	@Override
	public String getSDKVersion() {
		return "6.5";
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> devInfo) {
		try {
			mPublishID = devInfo.get("AdmobID");
			LogD("init AppInfo : " + mPublishID);
		} catch (Exception e) {
			LogE("initAppInfo, The format of appInfo is wrong", e);
		}
	}

	@Override
	public void showAds(Hashtable<String, String> info, int pos) {
	    try
	    {
	        String strType = info.get("AdmobType");
	        int adsType = Integer.parseInt(strType);

	        switch (adsType) {
	        case ADMOB_TYPE_BANNER:
	            {
	                String strSize = info.get("AdmobSizeEnum");
	                int sizeEnum = Integer.parseInt(strSize);
    	            showBannerAd(sizeEnum, pos);
                    break;
	            }
	        case ADMOB_TYPE_FULLSCREEN:
	            LogD("Now not support full screen view in Admob");
	            break;
	        default:
	            break;
	        }
	    } catch (Exception e) {
	        LogE("Error when show Ads ( " + info.toString() + " )", e);
	    }
	}

	@Override
	public void spendPoints(int points) {
		LogD("Admob not support spend points!");
	}

	@Override
	public void hideAds(Hashtable<String, String> info) {
	    try
        {
            String strType = info.get("AdmobType");
            int adsType = Integer.parseInt(strType);

            switch (adsType) {
            case ADMOB_TYPE_BANNER:
                hideBannerAd();
                break;
            case ADMOB_TYPE_FULLSCREEN:
                LogD("Now not support full screen view in Admob");
                break;
            default:
                break;
            }
        } catch (Exception e) {
            LogE("Error when hide Ads ( " + info.toString() + " )", e);
        }
	}

	private void showBannerAd(int sizeEnum, int pos) {
		final int curPos = pos;
		final int curSize = sizeEnum;

		PluginWrapper.runOnMainThread(new Runnable() {

			@Override
			public void run() {
				// destory the ad view before
				if (null != adView) {
					if (null != mWm) {
						mWm.removeView(adView);
					}
					adView.destroy();
					adView = null;
				}

				AdSize size = AdSize.BANNER;
				switch (curSize) {
                    case AdsAdmob.kSizeBanner:
                        size = AdSize.BANNER;
                        break;
                    case AdsAdmob.kSizeFullBanner:
                        size = AdSize.FULL_BANNER;
                        break;
                    case AdsAdmob.kSizeLargeBanner:
                        size = AdSize.LARGE_BANNER;
                        break;
                    case AdsAdmob.kSizeLeaderboard:
                        size = AdSize.LEADERBOARD;
                        break;
                    case AdsAdmob.kSizeMediumRectangle:
                        size = AdSize.MEDIUM_RECTANGLE;
                        break;
                    case AdsAdmob.kSizeSmartBanner:
                        size = AdSize.SMART_BANNER;
                        break;
                    case AdsAdmob.kSizeSkyscraper:
                        size = AdSize.WIDE_SKYSCRAPER;
                        break;
                    default:
                        break;
				}


				adView = new AdView(mContext);
                		adView.setAdUnitId(mPublishID);
				adView.setAdSize(size);
				
				AdRequest.Builder reqBuilder = new AdRequest.Builder(); 
				
				try {
					if (mTestDevices != null) {
						Iterator<String> ir = mTestDevices.iterator();
						while(ir.hasNext())
						{
							reqBuilder.addTestDevice(ir.next());
						}
					}
				} catch (Exception e) {
					LogE("Error during add test device", e);
				}
				
				adView.setAdListener(new AdmobAdsListener());
				adView.loadAd(reqBuilder.build());

				if (null == mWm) {
					mWm = (WindowManager) mContext.getSystemService("window");
				}
				AdsWrapper.addAdView(mWm, adView, curPos);
			}
		});
	}

	private void hideBannerAd() {
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				if (null != adView) {
					if (null != mWm) {
						mWm.removeView(adView);
					}
					adView.destroy();
					adView = null;
				}
			}
		});
	}

	public void addTestDevice(String deviceID) {
		LogD("addTestDevice invoked : " + deviceID);
		if (null == mTestDevices) {
			mTestDevices = new HashSet<String>();
		}
		mTestDevices.add(deviceID);
	}

	private class AdmobAdsListener extends AdListener {

		@Override
		public void onAdClosed() {
			LogD("onDismissScreen invoked");
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsDismissed, "Ads view dismissed!");
		}

		@Override
		public void onAdFailedToLoad(int errorCode) {
			int errorNo = AdsWrapper.RESULT_CODE_UnknownError;
			String errorMsg = "Unknow error";
			switch (errorCode) {
			case AdRequest.ERROR_CODE_NETWORK_ERROR:
				errorNo =  AdsWrapper.RESULT_CODE_NetworkError;
				errorMsg = "Network error";
				break;
			case AdRequest.ERROR_CODE_INVALID_REQUEST:
				errorNo = AdsWrapper.RESULT_CODE_NetworkError;
				errorMsg = "The ad request is invalid";
				break;
			case AdRequest.ERROR_CODE_NO_FILL:
				errorMsg = "The ad request is successful, but no ad was returned due to lack of ad inventory.";
				break;
			default:
				break;
			}
			LogD("failed to receive ad : " + errorNo + " , " + errorMsg);
			AdsWrapper.onAdsResult(mAdapter, errorNo, errorMsg);
		}

		@Override
		public void onAdLeftApplication() {
			LogD("onLeaveApplication invoked");
		}

		@Override
		public void onAdOpened() {
			LogD("onPresentScreen invoked");
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsShown, "Ads view shown!");
		}

		@Override
		public void onAdLoaded() {
			LogD("onReceiveAd invoked");
			AdsWrapper.onAdsResult(mAdapter, AdsWrapper.RESULT_CODE_AdsReceived, "Ads request received success!");
		}
	}

	@Override
	public String getPluginVersion() {
		return "0.2.0";
	}

    @Override
    public void queryPoints() {
        LogD("Admob not support query points!");
    }
}
