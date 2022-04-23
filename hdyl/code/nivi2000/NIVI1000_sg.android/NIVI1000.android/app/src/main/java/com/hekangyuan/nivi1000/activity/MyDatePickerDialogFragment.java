package com.hekangyuan.nivi1000.activity;

import com.ycuwq.datepicker.date.DatePickerDialogFragment;

public class MyDatePickerDialogFragment extends DatePickerDialogFragment {

    @Override
    protected void initChild() {
        super.initChild();
        mCancelButton.setTextSize(mCancelButton.getTextSize() + 5);
        mDecideButton.setTextSize(mDecideButton.getTextSize() + 5);
        mDatePicker.setShowCurtain(false);
    }
}
