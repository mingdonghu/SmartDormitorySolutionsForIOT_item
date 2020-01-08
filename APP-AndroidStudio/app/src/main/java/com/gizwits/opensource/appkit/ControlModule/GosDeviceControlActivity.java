package com.gizwits.opensource.appkit.ControlModule;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.content.Intent;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;
import android.widget.Toast;

import java.util.concurrent.ConcurrentHashMap;

import com.gizwits.gizwifisdk.api.GizWifiDevice;
import com.gizwits.gizwifisdk.enumration.GizWifiDeviceNetStatus;
import com.gizwits.gizwifisdk.enumration.GizWifiErrorCode;
import com.gizwits.opensource.appkit.CommonModule.GosDeploy;
import com.gizwits.opensource.appkit.R;
import com.gizwits.opensource.appkit.utils.HexStrUtils;
import com.gizwits.opensource.appkit.view.HexWatcher;

public class GosDeviceControlActivity extends GosControlModuleBaseActivity
		implements OnClickListener, OnEditorActionListener, OnSeekBarChangeListener {

	/** 设备列表传入的设备变量 */
	private GizWifiDevice mDevice;

	private Switch sw_bool_LED;
	private Switch sw_bool_IS_GetUpAlarm;
	private Switch sw_bool_IS_GoToBedAlarm;
	private Spinner sp_enum_FireMonitorState;
	private Spinner sp_enum_PowerMonitorState;
	private TextView tv_data_SetGetUpHour;
	private SeekBar sb_data_SetGetUpHour;
	private TextView tv_data_SetGetUpMinute;
	private SeekBar sb_data_SetGetUpMinute;
	private TextView tv_data_SetGoToBedHour;
	private SeekBar sb_data_SetGoToBedHour;
	private TextView tv_data_SetGoToBedMinute;
	private SeekBar sb_data_SetGoToBedMinute;
	private TextView tv_data_SetPowerMonitorVlaue;
	private SeekBar sb_data_SetPowerMonitorVlaue;
	private Spinner sp_enum_FireMonitor;
	private Spinner sp_enum_LED_status;
	private Spinner sp_enum_PowerMonitor;
	private TextView tv_data_DisplayPowerMonitorVlaue;

	private enum handler_key {

		/** 更新界面 */
		UPDATE_UI,

		DISCONNECT,
	}

	private Runnable mRunnable = new Runnable() {
		public void run() {
			if (isDeviceCanBeControlled()) {
				progressDialog.cancel();
			} else {
				toastDeviceNoReadyAndExit();
			}
		}

	};

	/** The handler. */
	Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			handler_key key = handler_key.values()[msg.what];
			switch (key) {
			case UPDATE_UI:
				updateUI();
				break;
			case DISCONNECT:
				toastDeviceDisconnectAndExit();
				break;
			}
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_gos_device_control);
		initDevice();
		setToolBar(true, getDeviceName());
		final Drawable add = getResources().getDrawable(R.drawable.common_setting_more);
		int color = GosDeploy.appConfig_Contrast();
		add.setColorFilter(color, PorterDuff.Mode.SRC_ATOP);
		mToolbar.setOverflowIcon(add);
		initView();
		initEvent();
	}

	private void initView() {
		
		sw_bool_LED = (Switch) findViewById(R.id.sw_bool_LED);
		sw_bool_IS_GetUpAlarm = (Switch) findViewById(R.id.sw_bool_IS_GetUpAlarm);
		sw_bool_IS_GoToBedAlarm = (Switch) findViewById(R.id.sw_bool_IS_GoToBedAlarm);
		sp_enum_FireMonitorState = (Spinner) findViewById(R.id.sp_enum_FireMonitorState);
		sp_enum_PowerMonitorState = (Spinner) findViewById(R.id.sp_enum_PowerMonitorState);
		tv_data_SetGetUpHour = (TextView) findViewById(R.id.tv_data_SetGetUpHour);
		sb_data_SetGetUpHour = (SeekBar) findViewById(R.id.sb_data_SetGetUpHour);
		tv_data_SetGetUpMinute = (TextView) findViewById(R.id.tv_data_SetGetUpMinute);
		sb_data_SetGetUpMinute = (SeekBar) findViewById(R.id.sb_data_SetGetUpMinute);
		tv_data_SetGoToBedHour = (TextView) findViewById(R.id.tv_data_SetGoToBedHour);
		sb_data_SetGoToBedHour = (SeekBar) findViewById(R.id.sb_data_SetGoToBedHour);
		tv_data_SetGoToBedMinute = (TextView) findViewById(R.id.tv_data_SetGoToBedMinute);
		sb_data_SetGoToBedMinute = (SeekBar) findViewById(R.id.sb_data_SetGoToBedMinute);
		tv_data_SetPowerMonitorVlaue = (TextView) findViewById(R.id.tv_data_SetPowerMonitorVlaue);
		sb_data_SetPowerMonitorVlaue = (SeekBar) findViewById(R.id.sb_data_SetPowerMonitorVlaue);
		sp_enum_FireMonitor = (Spinner) findViewById(R.id.sp_enum_FireMonitor);
		sp_enum_LED_status = (Spinner) findViewById(R.id.sp_enum_LED_status);
		sp_enum_PowerMonitor = (Spinner) findViewById(R.id.sp_enum_PowerMonitor);
		tv_data_DisplayPowerMonitorVlaue = (TextView) findViewById(R.id.tv_data_DisplayPowerMonitorVlaue);
	}

	private void initEvent() {

		sw_bool_LED.setOnClickListener(this);
		sw_bool_IS_GetUpAlarm.setOnClickListener(this);
		sw_bool_IS_GoToBedAlarm.setOnClickListener(this);
		sp_enum_FireMonitorState.setSelection(0, false);
		sp_enum_FireMonitorState.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

				if (position != data_FireMonitorState) {
					sendCommand(KEY_FIREMONITORSTATE, position);
					data_FireMonitorState = position;
				}
			}

			@Override
			public void onNothingSelected(AdapterView<?> parent) {
			}
		});
		sp_enum_PowerMonitorState.setSelection(0, false);
		sp_enum_PowerMonitorState.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

				if (position != data_PowerMonitorState) {
					sendCommand(KEY_POWERMONITORSTATE, position);
					data_PowerMonitorState = position;
				}
			}

			@Override
			public void onNothingSelected(AdapterView<?> parent) {
			}
		});
		sb_data_SetGetUpHour.setOnSeekBarChangeListener(this);
		sb_data_SetGetUpMinute.setOnSeekBarChangeListener(this);
		sb_data_SetGoToBedHour.setOnSeekBarChangeListener(this);
		sb_data_SetGoToBedMinute.setOnSeekBarChangeListener(this);
		sb_data_SetPowerMonitorVlaue.setOnSeekBarChangeListener(this);
		sp_enum_FireMonitor.setEnabled(false);
		sp_enum_LED_status.setEnabled(false);
		sp_enum_PowerMonitor.setEnabled(false);
	
	}

	private void initDevice() {
		Intent intent = getIntent();
		mDevice = (GizWifiDevice) intent.getParcelableExtra("GizWifiDevice");
		mDevice.setListener(gizWifiDeviceListener);
		Log.i("Apptest", mDevice.getDid());
	}

	private String getDeviceName() {
		if (TextUtils.isEmpty(mDevice.getAlias())) {
			return mDevice.getProductName();
		}
		return mDevice.getAlias();
	}

	@Override
	protected void onResume() {
		super.onResume();
		getStatusOfDevice();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		mHandler.removeCallbacks(mRunnable);
		// 退出页面，取消设备订阅
		mDevice.setSubscribe(false);
		mDevice.setListener(null);
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.sw_bool_LED:
			sendCommand(KEY_LED, sw_bool_LED.isChecked());
			break;
		case R.id.sw_bool_IS_GetUpAlarm:
			sendCommand(KEY_IS_GETUPALARM, sw_bool_IS_GetUpAlarm.isChecked());
			break;
		case R.id.sw_bool_IS_GoToBedAlarm:
			sendCommand(KEY_IS_GOTOBEDALARM, sw_bool_IS_GoToBedAlarm.isChecked());
			break;
		default:
			break;
		}
	}

	/*
	 * ========================================================================
	 * EditText 点击键盘“完成”按钮方法
	 * ========================================================================
	 */
	@Override
	public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {

		switch (v.getId()) {
		default:
			break;
		}
		hideKeyBoard();
		return false;

	}
	
	/*
	 * ========================================================================
	 * seekbar 回调方法重写
	 * ========================================================================
	 */
	@Override
	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
		
		switch (seekBar.getId()) {
		case R.id.sb_data_SetGetUpHour:
			tv_data_SetGetUpHour.setText(formatValue((progress + SETGETUPHOUR_OFFSET) * SETGETUPHOUR_RATIO + SETGETUPHOUR_ADDITION, 1));
			break;
		case R.id.sb_data_SetGetUpMinute:
			tv_data_SetGetUpMinute.setText(formatValue((progress + SETGETUPMINUTE_OFFSET) * SETGETUPMINUTE_RATIO + SETGETUPMINUTE_ADDITION, 1));
			break;
		case R.id.sb_data_SetGoToBedHour:
			tv_data_SetGoToBedHour.setText(formatValue((progress + SETGOTOBEDHOUR_OFFSET) * SETGOTOBEDHOUR_RATIO + SETGOTOBEDHOUR_ADDITION, 1));
			break;
		case R.id.sb_data_SetGoToBedMinute:
			tv_data_SetGoToBedMinute.setText(formatValue((progress + SETGOTOBEDMINUTE_OFFSET) * SETGOTOBEDMINUTE_RATIO + SETGOTOBEDMINUTE_ADDITION, 1));
			break;
		case R.id.sb_data_SetPowerMonitorVlaue:
			tv_data_SetPowerMonitorVlaue.setText(formatValue((progress + SETPOWERMONITORVLAUE_OFFSET) * SETPOWERMONITORVLAUE_RATIO + SETPOWERMONITORVLAUE_ADDITION, 1));
			break;
		default:
			break;
		}
	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar) {

	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar) {
		switch (seekBar.getId()) {
		case R.id.sb_data_SetGetUpHour:
			sendCommand(KEY_SETGETUPHOUR, (seekBar.getProgress() + SETGETUPHOUR_OFFSET ) * SETGETUPHOUR_RATIO + SETGETUPHOUR_ADDITION);
			break;
		case R.id.sb_data_SetGetUpMinute:
			sendCommand(KEY_SETGETUPMINUTE, (seekBar.getProgress() + SETGETUPMINUTE_OFFSET ) * SETGETUPMINUTE_RATIO + SETGETUPMINUTE_ADDITION);
			break;
		case R.id.sb_data_SetGoToBedHour:
			sendCommand(KEY_SETGOTOBEDHOUR, (seekBar.getProgress() + SETGOTOBEDHOUR_OFFSET ) * SETGOTOBEDHOUR_RATIO + SETGOTOBEDHOUR_ADDITION);
			break;
		case R.id.sb_data_SetGoToBedMinute:
			sendCommand(KEY_SETGOTOBEDMINUTE, (seekBar.getProgress() + SETGOTOBEDMINUTE_OFFSET ) * SETGOTOBEDMINUTE_RATIO + SETGOTOBEDMINUTE_ADDITION);
			break;
		case R.id.sb_data_SetPowerMonitorVlaue:
			sendCommand(KEY_SETPOWERMONITORVLAUE, (seekBar.getProgress() + SETPOWERMONITORVLAUE_OFFSET ) * SETPOWERMONITORVLAUE_RATIO + SETPOWERMONITORVLAUE_ADDITION);
			break;
		default:
			break;
		}
	}

	/*
	 * ========================================================================
	 * 菜单栏
	 * ========================================================================
	 */
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.device_more, menu);
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {

		case R.id.action_setDeviceInfo:
			setDeviceInfo();
			break;

		case R.id.action_getHardwareInfo:
			if (mDevice.isLAN()) {
				mDevice.getHardwareInfo();
			} else {
				myToast("只允许在局域网下获取设备硬件信息！");
			}
			break;

		case R.id.action_getStatu:
			mDevice.getDeviceStatus();
			break;

		default:
			break;
		}

		return super.onOptionsItemSelected(item);
	}

	/**
	 * Description:根据保存的的数据点的值来更新UI
	 */
	protected void updateUI() {
		
		sw_bool_LED.setChecked(data_LED);
		sw_bool_IS_GetUpAlarm.setChecked(data_IS_GetUpAlarm);
		sw_bool_IS_GoToBedAlarm.setChecked(data_IS_GoToBedAlarm);
		sp_enum_FireMonitorState.setSelection(data_FireMonitorState, true);
		sp_enum_PowerMonitorState.setSelection(data_PowerMonitorState, true);
		tv_data_SetGetUpHour.setText(data_SetGetUpHour+"");
		sb_data_SetGetUpHour.setProgress((int)((data_SetGetUpHour - SETGETUPHOUR_ADDITION) / SETGETUPHOUR_RATIO - SETGETUPHOUR_OFFSET));
		tv_data_SetGetUpMinute.setText(data_SetGetUpMinute+"");
		sb_data_SetGetUpMinute.setProgress((int)((data_SetGetUpMinute - SETGETUPMINUTE_ADDITION) / SETGETUPMINUTE_RATIO - SETGETUPMINUTE_OFFSET));
		tv_data_SetGoToBedHour.setText(data_SetGoToBedHour+"");
		sb_data_SetGoToBedHour.setProgress((int)((data_SetGoToBedHour - SETGOTOBEDHOUR_ADDITION) / SETGOTOBEDHOUR_RATIO - SETGOTOBEDHOUR_OFFSET));
		tv_data_SetGoToBedMinute.setText(data_SetGoToBedMinute+"");
		sb_data_SetGoToBedMinute.setProgress((int)((data_SetGoToBedMinute - SETGOTOBEDMINUTE_ADDITION) / SETGOTOBEDMINUTE_RATIO - SETGOTOBEDMINUTE_OFFSET));
		tv_data_SetPowerMonitorVlaue.setText(data_SetPowerMonitorVlaue+"");
		sb_data_SetPowerMonitorVlaue.setProgress((int)((data_SetPowerMonitorVlaue - SETPOWERMONITORVLAUE_ADDITION) / SETPOWERMONITORVLAUE_RATIO - SETPOWERMONITORVLAUE_OFFSET));
		sp_enum_FireMonitor.setSelection(data_FireMonitor, true);
		sp_enum_LED_status.setSelection(data_LED_status, true);
		sp_enum_PowerMonitor.setSelection(data_PowerMonitor, true);
		tv_data_DisplayPowerMonitorVlaue.setText(data_DisplayPowerMonitorVlaue+"");
	
	}

	private void setEditText(EditText et, Object value) {
		et.setText(value.toString());
		et.setSelection(value.toString().length());
		et.clearFocus();
	}

	/**
	 * Description:页面加载后弹出等待框，等待设备可被控制状态回调，如果一直不可被控，等待一段时间后自动退出界面
	 */
	private void getStatusOfDevice() {
		// 设备是否可控
		if (isDeviceCanBeControlled()) {
			// 可控则查询当前设备状态
			mDevice.getDeviceStatus();
		} else {
			// 显示等待栏
			progressDialog.show();
			if (mDevice.isLAN()) {
				// 小循环10s未连接上设备自动退出
				mHandler.postDelayed(mRunnable, 10000);
			} else {
				// 大循环20s未连接上设备自动退出
				mHandler.postDelayed(mRunnable, 20000);
			}
		}
	}

	/**
	 * 发送指令,下发单个数据点的命令可以用这个方法
	 * 
	 * <h3>注意</h3>
	 * <p>
	 * 下发多个数据点命令不能用这个方法多次调用，一次性多次调用这个方法会导致模组无法正确接收消息，参考方法内注释。
	 * </p>
	 * 
	 * @param key
	 *            数据点对应的标识名
	 * @param value
	 *            需要改变的值
	 */
	private void sendCommand(String key, Object value) {
		if (value == null) {
			return;
		}
		int sn = 5;
		ConcurrentHashMap<String, Object> hashMap = new ConcurrentHashMap<String, Object>();
		hashMap.put(key, value);
		// 同时下发多个数据点需要一次性在map中放置全部需要控制的key，value值
		// hashMap.put(key2, value2);
		// hashMap.put(key3, value3);
		mDevice.write(hashMap, sn);
		Log.i("liang", "下发命令：" + hashMap.toString());
	}

	private boolean isDeviceCanBeControlled() {
		return mDevice.getNetStatus() == GizWifiDeviceNetStatus.GizDeviceControlled;
	}

	private void toastDeviceNoReadyAndExit() {
		Toast.makeText(this, "设备无响应，请检查设备是否正常工作", Toast.LENGTH_SHORT).show();
		finish();
	}

	private void toastDeviceDisconnectAndExit() {
		Toast.makeText(GosDeviceControlActivity.this, "连接已断开", Toast.LENGTH_SHORT).show();
		finish();
	}

	/**
	 * 展示设备硬件信息
	 * 
	 * @param hardwareInfo
	 */
	private void showHardwareInfo(String hardwareInfo) {
		String hardwareInfoTitle = "设备硬件信息";
		new AlertDialog.Builder(this).setTitle(hardwareInfoTitle).setMessage(hardwareInfo)
				.setPositiveButton(R.string.besure, null).show();
	}

	/**
	 * Description:设置设备别名与备注
	 */
	private void setDeviceInfo() {

		final Dialog mDialog = new AlertDialog.Builder(this,R.style.edit_dialog_style).setView(new EditText(this)).create();
		mDialog.show();

		Window window = mDialog.getWindow();
		window.setContentView(R.layout.alert_gos_set_device_info);
		WindowManager.LayoutParams layoutParams = window.getAttributes();
		layoutParams.width = WindowManager.LayoutParams.MATCH_PARENT;
		window.setAttributes(layoutParams);
		final EditText etAlias;
		final EditText etRemark;
		etAlias = (EditText) window.findViewById(R.id.etAlias);
		etRemark = (EditText) window.findViewById(R.id.etRemark);

		LinearLayout llNo, llSure;
		llNo = (LinearLayout) window.findViewById(R.id.llNo);
		llSure = (LinearLayout) window.findViewById(R.id.llSure);

		if (!TextUtils.isEmpty(mDevice.getAlias())) {
			setEditText(etAlias, mDevice.getAlias());
		}
		if (!TextUtils.isEmpty(mDevice.getRemark())) {
			setEditText(etRemark, mDevice.getRemark());
		}

		llNo.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				mDialog.dismiss();
			}
		});

		llSure.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (TextUtils.isEmpty(etRemark.getText().toString())
						&& TextUtils.isEmpty(etAlias.getText().toString())) {
					myToast("请输入设备别名或备注！");
					return;
				}
				mDevice.setCustomInfo(etRemark.getText().toString(), etAlias.getText().toString());
				mDialog.dismiss();
				String loadingText = (String) getText(R.string.loadingtext);
				progressDialog.setMessage(loadingText);
				progressDialog.show();
			}
		});

		mDialog.setOnDismissListener(new OnDismissListener() {
			@Override
			public void onDismiss(DialogInterface dialog) {
				hideKeyBoard();
			}
		});
	}
	
	/*
	 * 获取设备硬件信息回调
	 */
	@Override
	protected void didGetHardwareInfo(GizWifiErrorCode result, GizWifiDevice device,
			ConcurrentHashMap<String, String> hardwareInfo) {
		super.didGetHardwareInfo(result, device, hardwareInfo);
		StringBuffer sb = new StringBuffer();
		if (GizWifiErrorCode.GIZ_SDK_SUCCESS != result) {
			myToast("获取设备硬件信息失败：" + result.name());
		} else {
			sb.append("Wifi Hardware Version:" + hardwareInfo.get(WIFI_HARDVER_KEY) + "\r\n");
			sb.append("Wifi Software Version:" + hardwareInfo.get(WIFI_SOFTVER_KEY) + "\r\n");
			sb.append("MCU Hardware Version:" + hardwareInfo.get(MCU_HARDVER_KEY) + "\r\n");
			sb.append("MCU Software Version:" + hardwareInfo.get(MCU_SOFTVER_KEY) + "\r\n");
			sb.append("Wifi Firmware Id:" + hardwareInfo.get(WIFI_FIRMWAREID_KEY) + "\r\n");
			sb.append("Wifi Firmware Version:" + hardwareInfo.get(WIFI_FIRMWAREVER_KEY) + "\r\n");
			sb.append("Product Key:" + "\r\n" + hardwareInfo.get(PRODUCT_KEY) + "\r\n");

			// 设备属性
			sb.append("Device ID:" + "\r\n" + mDevice.getDid() + "\r\n");
			sb.append("Device IP:" + mDevice.getIPAddress() + "\r\n");
			sb.append("Device MAC:" + mDevice.getMacAddress() + "\r\n");
		}
		showHardwareInfo(sb.toString());
	}
	
	/*
	 * 设置设备别名和备注回调
	 */
	@Override
	protected void didSetCustomInfo(GizWifiErrorCode result, GizWifiDevice device) {
		super.didSetCustomInfo(result, device);
		if (GizWifiErrorCode.GIZ_SDK_SUCCESS == result) {
			myToast("设置成功");
			progressDialog.cancel();
			finish();
		} else {
			myToast("设置失败：" + result.name());
		}
	}

	/*
	 * 设备状态改变回调，只有设备状态为可控才可以下发控制命令
	 */
	@Override
	protected void didUpdateNetStatus(GizWifiDevice device, GizWifiDeviceNetStatus netStatus) {
		super.didUpdateNetStatus(device, netStatus);
		if (netStatus == GizWifiDeviceNetStatus.GizDeviceControlled) {
			mHandler.removeCallbacks(mRunnable);
			progressDialog.cancel();
		} else {
			mHandler.sendEmptyMessage(handler_key.DISCONNECT.ordinal());
		}
	}
	
	/*
	 * 设备上报数据回调，此回调包括设备主动上报数据、下发控制命令成功后设备返回ACK
	 */
	@Override
	protected void didReceiveData(GizWifiErrorCode result, GizWifiDevice device,
			ConcurrentHashMap<String, Object> dataMap, int sn) {
		super.didReceiveData(result, device, dataMap, sn);
		Log.i("liang", "接收到数据");
		if (result == GizWifiErrorCode.GIZ_SDK_SUCCESS && dataMap.get("data") != null) {
			getDataFromReceiveDataMap(dataMap);
			mHandler.sendEmptyMessage(handler_key.UPDATE_UI.ordinal());
		}
	}

}