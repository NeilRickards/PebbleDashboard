module.exports = [
  {
    "type": "heading",
    "defaultValue": "Dashboard Configuration"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Time Offsets"
      },
      {
        "type": "slider",
        "messageKey": "SETTING_TIME_OFFSET0",
        "defaultValue": -8,
        "label": "Left",
        "min": -12,
        "max": 12,
        "step": 0.5
      },
      {
        "type": "slider",
        "messageKey": "SETTING_TIME_OFFSET1",
        "defaultValue": -5,
        "label": "Middle",
        "min": -12,
        "max": 12,
        "step": 0.5
      },
      {
        "type": "slider",
        "messageKey": "SETTING_TIME_OFFSET2",
        "defaultValue": 8,
        "label": "Right",
        "min": -12,
        "max": 12,
        "step": 0.5
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }  
];
