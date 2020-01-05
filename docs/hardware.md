# Firebase Structure

We need to know the state of some elements of the hardware. Such as which was the last eppendorf used and how many
times the same cell has been used without cleaning.

**All this values must be reset in the dashboard**. They will be updated from the microcontroller.

```json5
{
  "hardware": {
      "content": {
          // (int) integer value of which is the next slot ready to use
          "eppendorf_idx": null,
          // (int) how many times the cell has been used without cleaning
          "n_cell_usages": null,
          // (datetime) how much time the cell has been used without cleaning
          "time_cell_usages": null,

      }
  } 
}
```